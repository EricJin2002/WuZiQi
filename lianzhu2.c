#include "wuziqi.h"

void lianzhu_calc_find(int *pattern,int *x0,int d,int *n,bool *blank,bool consider_ban,int whom){
    if(!consider_ban){
        while((*x0<=16&&*x0>=0||(*blank=false))&&*n<=4){
            if(((*pattern>>*x0)&0b11)==(whom^1)){
                //敌方
                *blank=false;
                break;
            }else if(((*pattern>>*x0)&0b11)!=(whom&0b11)){
                //为空
                break;
            }
            *x0+=d;
            (*n)++;
        }
    }else{
        while((*x0<=16&&*x0>=0||(*blank=false))&&*n<=4){
            if(((*pattern>>*x0)&0b11)==(whom^1)){
                //敌方
                *blank=false;
                break;
            }else if(((*pattern>>*x0)&0b11)==(EMPTY&0b11)){
                //为空
                break;
            }else if(((*pattern>>*x0)&0b11)==(BAN&0b11)){
                //己方禁手
                *blank=false;   
                break;
            }
            *x0+=d;
            (*n)++;
        }
    }
}

int lianzhu_calc_gen(int *pattern,bool consider_ban,int whom){
    int i=1,j=1;
    int i_x=2*5;
    int j_x=2*3;
    bool i_blank=true;
    bool j_blank=true;
    lianzhu_calc_find(pattern,&i_x,2,&i,&i_blank,consider_ban,whom);
    lianzhu_calc_find(pattern,&j_x,-2,&j,&j_blank,consider_ban,whom);
    if(i+j>=7){
        return CHANG_LIAN;
    }else if(i+j==6){//连五11111
        return CHENG_5;
    }else if(i+j==5){//连四1111
        if(i_blank&&j_blank) return HUO_4;//活四011110
        if(i_blank||j_blank) return CHONG_4;//冲四011112
        return SI_4;//211112
    }else{
        int ii_x=i_x+2,ii=i+1;
        bool ii_blank=i_blank;
        int jj_x=j_x-2,jj=j+1;
        bool jj_blank=j_blank;
        if(i_blank){
            lianzhu_calc_find(pattern,&ii_x,2,&ii,&ii_blank,consider_ban,whom);
        }
        if(j_blank){
            lianzhu_calc_find(pattern,&jj_x,-2,&jj,&jj_blank,consider_ban,whom);
        }
        if(i+j==4){//连三111
            if(ii+j>5&&jj+i>5) return _4_4;//双四1011101
            if(ii+j>5||jj+i>5) return CHONG_4;//冲四10111
            if(ii_blank&&jj_blank) return HUO_3;//活三0011100
            if(ii_blank&&j_blank||jj_blank&&i_blank) return TIAO_HUO_3;//跳活三0011102
            if(i_blank&&j_blank) return MIAN_3;//眠三2011102
            else if(jj_blank||ii_blank) return MIAN_3;//眠三211100
            return 0;
        }
        if(ii+j>5&&jj+i>5) return _4_4;//双四11011011,111010111
        if(ii+j>5||jj+i>5) return CHONG_4;//冲四11011,11101
        if(ii+j>4&&jj+i>4&&ii_blank&&jj_blank) return HUO_3;//活三01011010,011010110    
        if(ii_blank&&j_blank&&ii+j>4||
            jj_blank&&i_blank&&jj+i>4) return TIAO_HUO_3;//跳活三010110
        if((ii_blank||j_blank)&&ii+j>4||
            (jj_blank||i_blank)&&jj+i>4) return MIAN_3;//眠三010112,210110
        if(ii_blank&&j_blank&&ii+j>3||
            jj_blank&&i_blank&&jj+i>3) return HUO_2;//活二001010,001100
        if((ii_blank||j_blank)&&ii+j>3||
            (jj_blank||i_blank)&&jj+i>3) return MIAN_2;//眠二20110，21010，21100
        return 0;
    }
}

void lianzhu_calc_init(){
    for(int pattern=0;pattern<=262143;pattern++){
        bool consider_ban=false;
        do{
            lianzhu_calc_map[pattern][consider_ban][BLACK]=lianzhu_calc_gen(&pattern,consider_ban,BLACK);
            lianzhu_calc_map[pattern][consider_ban][WHITE]=lianzhu_calc_gen(&pattern,consider_ban,WHITE);
        }while(consider_ban^=1);
    }
}

int lianzhu_calc(int x0,int y0,bool color,int dir,bool consider_ban,int par_k){
    //int rem=board[x0][y0];
    //board[x0][y0]=color+2;

    int (**boardptr)[23]=((par_k==-1)?&board:&(par_board[par_k]));

    int dx=0,dy=0;
    switch (dir){
    case 1:dx=dy=1;break;
    case 2:dx=1;break;
    case 3:dx=1;dy=-1;break;
    case 4:dy=-1;break;
    default:break;
    }
    int x3=x0-dx*4;
    int y3=y0-dy*4;
    int pattern=0;
    for(int i=-4;i<=4;i++,x3+=dx,y3+=dy){
        pattern<<=2;
        if((*boardptr)[x3][y3]==EDGE){
            pattern|=(color^1);
            continue;
        }
        pattern|=((*boardptr)[x3][y3]&0b11);
    }
    //board[x0][y0]=rem;
    return lianzhu_calc_map[pattern][consider_ban&&color][color];
}

bool lianzhu_judge_ban(int x0,int y0,int par_k){

    int (*lianzhuptr)[16][16][5]=((par_k==-1)?lianzhu:par_lianzhu[par_k]);

    int _3=0;
    int _4=0;
    bool ban=false;
    bool win=false;
    for(int dir_4=1;dir_4<=4;dir_4++){
        switch ((*lianzhuptr)[x0][y0][dir_4]){
        case CHANG_LIAN: ban=true;break;
        case CHENG_5: win=true;break;
        case HUO_4: _4++;break;
        case CHONG_4: _4++;break;
        //case SI_4: break;//really?
        case _4_4: ban=true;break;
        case HUO_3: _3++;break;
        case TIAO_HUO_3: _3++;break;
        //case MIAN_3: break;
        default: break;
        }
    }
    if(win) return false;
    if(ban) return true;
    if(_3>=2||_4>=2) return true;
    return false;
}

void lianzhu_refresh_ban(){
    for(int i=1;i<=15;i++){
        for(int j=1;j<=15;j++){
            if(board[i][j]<0){
                lianzhu[i][j][1]=lianzhu_calc(i,j,BLACK,1,false,-1);
                lianzhu[i][j][2]=lianzhu_calc(i,j,BLACK,2,false,-1);
                lianzhu[i][j][3]=lianzhu_calc(i,j,BLACK,3,false,-1);
                lianzhu[i][j][4]=lianzhu_calc(i,j,BLACK,4,false,-1);
                board[i][j]=lianzhu_judge_ban(i,j,-1)?BAN:EMPTY;
            }
        }
    }
}
