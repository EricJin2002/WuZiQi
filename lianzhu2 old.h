#pragma once
#include "wuziqi.h"

void lianzhu_find(int *x0,int *y0,int dx,int dy,int *n,bool *blank,bool *color,bool consider_ban){
    if(!consider_ban||!*color){
        while((*x0<=15&&*x0>=1&&*y0<=15&&*y0>=1||(*blank=false))&&*n<=4){
            if(board[*x0][*y0]<=0){
                //为空
                break;
            }else if((board[*x0][*y0]&1)!=*color){
                //敌方
                *blank=false;
                break;
            }
            *x0+=dx;
            *y0+=dy;
            (*n)++;
        }
    }else{
        while((*x0<=15&&*x0>=1&&*y0<=15&&*y0>=1||(*blank=false))&&*n<=4){
            if(board[*x0][*y0]<=0){
                if(!board[*x0][*y0]) break;     //为空
                *blank=false;   //己方禁手
                break;
            }else if((board[*x0][*y0]&1)!=*color){
                //敌方
                *blank=false;
                break;
            }
            *x0+=dx;
            *y0+=dy;
            (*n)++;
        }
    }
}
#define CHANG_LIAN 6
#define CHENG_5 5
#define HUO_4 4
#define CHONG_4 104
#define SI_4 40
//#define _3_4 34
#define _4_4 44
#define HUO_3 3
#define TIAO_HUO_3 103
#define MIAN_3 30
#define HUO_2 2
#define MIAN_2 20

int lianzhu_calc(int x0,int y0,bool color,int dir,bool consider_ban){
    int dx=0,dy=0;
    switch (dir){
    case 1:dx=dy=1;break;
    case 2:dx=1;break;
    case 3:dx=1;dy=-1;break;
    case 4:dy=-1;break;
    default:break;
    }
    int i=1,j=1;
    int i_x=x0+dx,i_y=y0+dy;
    int j_x=x0-dx,j_y=y0-dy;
    bool i_blank=true;
    bool j_blank=true;
    lianzhu_find(&i_x,&i_y,dx,dy,&i,&i_blank,&color,consider_ban);
    lianzhu_find(&j_x,&j_y,-dx,-dy,&j,&j_blank,&color,consider_ban);
    if(i+j>=7){
        return CHANG_LIAN;
    }else if(i+j==6){//连五11111
        return CHENG_5;
    }else if(i+j==5){//连四1111
        if(i_blank&&j_blank) return HUO_4;//活四011110
        if(i_blank||j_blank) return CHONG_4;//冲四011112
        return SI_4;//211112
    }else{
        int ii_x=i_x+dx,ii_y=i_y+dy,ii=i+1;
        bool ii_blank=i_blank;
        int jj_x=j_x-dx,jj_y=j_y-dy,jj=j+1;
        bool jj_blank=j_blank;
        if(i_blank){
            lianzhu_find(&ii_x,&ii_y,dx,dy,&ii,&ii_blank,&color,consider_ban);
        }
        if(j_blank){
            lianzhu_find(&jj_x,&jj_y,-dx,-dy,&jj,&jj_blank,&color,consider_ban);
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

int lianzhu[16][16][5];
bool lianzhu_judge_ban(int x0,int y0){
    int _3=0;
    int _4=0;
    bool ban=false;
    bool win=false;
    for(int dir_4=1;dir_4<=4;dir_4++){
        switch (lianzhu[x0][y0][dir_4]){
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
            if(board[i][j]<=0){
                lianzhu[i][j][1]=lianzhu_calc(i,j,black,1,false);
                lianzhu[i][j][2]=lianzhu_calc(i,j,black,2,false);
                lianzhu[i][j][3]=lianzhu_calc(i,j,black,3,false);
                lianzhu[i][j][4]=lianzhu_calc(i,j,black,4,false);
                board[i][j]=lianzhu_judge_ban(i,j)?-2:0;
            }
        }
    }
}


void lianzhu_refresh_ban_near(int x0,int y0){
    for(int dir_8=1;dir_8<=8;dir_8++){
        int dx=0,dy=0;
        switch (dir_8){
        case 1:dx=dy=1;break;
        case 2:dx=1;break;
        case 3:dx=1;dy=-1;break;
        case 4:dy=-1;break;
        case 5:dx=dy=-1;break;
        case 6:dx=-1;break;
        case 7:dx=-1;dy=1;break;
        case 8:dy=1;break;
        default:break;
        }
        int x1,y1;
        for(int k=1;k<=5;k++){
            x1=x0+k*dx;
            y1=y0+k*dy;
            if(x1>=1&&x1<=15&&y1>=1&&y1<=15){
                if(board[x1][y1]<=0){
                    switch (dir_8){
                        case 1:case 5:lianzhu[x1][y1][1]=lianzhu_calc(x1,y1,black,1,false);break;
                        case 2:case 6:lianzhu[x1][y1][2]=lianzhu_calc(x1,y1,black,2,false);break;
                        case 3:case 7:lianzhu[x1][y1][3]=lianzhu_calc(x1,y1,black,3,false);break;
                        case 4:case 8:lianzhu[x1][y1][4]=lianzhu_calc(x1,y1,black,4,false);break;
                    }
                    board[x1][y1]=lianzhu_judge_ban(x1,y1)?-2:0;
                }
            }else{
                break;
            }
        }
    }
}