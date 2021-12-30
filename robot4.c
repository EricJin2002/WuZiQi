#include "wuziqi.h"

void fg4_value_map_init(){
    for(int i=0;i<TYPES_TOT;i++){
        switch (i){
        case CHANG_LIAN: fg4_value_map[i]= 5000000;break;
        case CHENG_5: fg4_value_map[i]= 5000000;break;
        case HUO_4: fg4_value_map[i]= 100000;break;
        case CHONG_4: fg4_value_map[i]= 10000;break;
        case SI_4: fg4_value_map[i]= 0;break;
        case _4_4: fg4_value_map[i]= 20000;break;
        case HUO_3: fg4_value_map[i]= 8000;break;
        case TIAO_HUO_3: fg4_value_map[i]= 7000;break;
        case MIAN_3: fg4_value_map[i]= 500;break;
        case HUO_2: fg4_value_map[i]= 50;break;
        case MIAN_2: fg4_value_map[i]= 10;break;
        default: fg4_value_map[i]= 0;break;
        }
    }
}

void fg4_refresh_value(int x0,int y0,int par_k){//par_k=-1 if using real board and lianzhu array
    //if(ban_black) lianzhu_refresh_ban_near(x0,y0);
    //Update: 211217
    //如果下边lianzhu_calc_map第二维度使用false，即不考虑禁手
    //则刷新禁手可以和刷新赋值同步进行

    //DONE: 211127 
    //注：211112的那版程序这里有问题，这句是必须的。
    //原因在于robot5调用robot4时，若每层落子点附近的禁手点不刷新，其效果会累积。
    //robot4自身使用时没这个问题，因为棋盘每轮稳定刷新禁手点。
    
    int (*value)[16][11]=((par_k==-1)?fg6_value:par_fg6_value[par_k]);
    int (**boardptr)[23]=((par_k==-1)?&board:&(par_board[par_k]));
    int (*lianzhuptr)[16][16][5]=((par_k==-1)?lianzhu:par_lianzhu[par_k]);

    for(int dir_4=1;dir_4<=4;dir_4++){
        int dx=0,dy=0;
        switch (dir_4){
        case 1:dx=dy=1;break;
        case 2:dx=1;break;
        case 3:dx=1;dy=-1;break;
        case 4:dy=-1;break;
        default:break;
        }
        //int color=WHITE;
        //do{
            int pattern_pos_white=0;
            int pattern_pos_black=0;
            int x3=x0-dx*4;
            int y3=y0-dy*4;
            for(int i=-4;i<=4;i++,x3+=dx,y3+=dy){
                pattern_pos_white<<=2;
                pattern_pos_black<<=2;
                if((*boardptr)[x3][y3]==EDGE){
                    pattern_pos_white|=(WHITE^1);
                    pattern_pos_black|=(BLACK^1);
                    continue;
                }
                pattern_pos_white|=((*boardptr)[x3][y3]&0b11);
                pattern_pos_black|=((*boardptr)[x3][y3]&0b11);
            }
            int pattern_neg_white=pattern_pos_white;
            int pattern_neg_black=pattern_pos_black;


            int x1=x0;
            int y1=y0;
            for(int k=1;k<=4;k++){
                x1+=dx;
                y1+=dy;
                if((*boardptr)[x1][y1]!=EDGE){
                    pattern_pos_white<<=2;
                    pattern_pos_black<<=2;
                    pattern_pos_white&=((1<<18)-1);
                    pattern_pos_black&=((1<<18)-1);
                    if((*boardptr)[x1+4*dx][y1+4*dy]==EDGE){
                        pattern_pos_white|=(WHITE^1);
                        pattern_pos_black|=(BLACK^1);
                    }else{
                        pattern_pos_black|=((*boardptr)[x1+4*dx][y1+4*dy]&0b11);
                        pattern_pos_white|=((*boardptr)[x1+4*dx][y1+4*dy]&0b11);
                    }
                    
                    if((*boardptr)[x1][y1]<0){
                        (*lianzhuptr)[x1][y1][dir_4]=lianzhu_calc_map[pattern_pos_black][false][BLACK];
                        (*boardptr)[x1][y1]=lianzhu_judge_ban(x1,y1,par_k)?BAN:EMPTY;
                    }

                    value[x1][y1][10-WHITE]-=value[x1][y1][dir_4+4*WHITE];
                    value[x1][y1][10-BLACK]-=value[x1][y1][dir_4+4*BLACK];
                    value[x1][y1][dir_4+4*WHITE]=fg4_value_map[lianzhu_calc_map[pattern_pos_white&((-1)^(0b11<<8))|(WHITE<<8)][false/*ban_black&&WHITE*/][WHITE]];
                    value[x1][y1][dir_4+4*BLACK]=fg4_value_map[lianzhu_calc_map[pattern_pos_black&((-1)^(0b11<<8))|(BLACK<<8)][false/*ban_black&&BLACK*/][BLACK]];
                    value[x1][y1][10-WHITE]+=value[x1][y1][dir_4+4*WHITE];
                    value[x1][y1][10-BLACK]+=value[x1][y1][dir_4+4*BLACK];
                    value[x1][y1][0]=value[x1][y1][10-WHITE]+
                        ((ban_black&&board[x1][y1]==BAN)?0:value[x1][y1][10-BLACK]);
                    
                }else{
                    break;
                }
            }

            x1=x0;
            y1=y0;
            for(int k=1;k<=4;k++){
                x1-=dx;
                y1-=dy;
                if((*boardptr)[x1][y1]!=EDGE){
                    pattern_neg_white>>=2;
                    pattern_neg_black>>=2;
                    if((*boardptr)[x1-4*dx][y1-4*dy]==EDGE){
                        pattern_neg_white|=((WHITE^1)<<16);
                        pattern_neg_black|=((BLACK^1)<<16);
                    }else{
                        pattern_neg_white|=(((*boardptr)[x1-4*dx][y1-4*dy]&0b11)<<16);
                        pattern_neg_black|=(((*boardptr)[x1-4*dx][y1-4*dy]&0b11)<<16);
                    }

                    if((*boardptr)[x1][y1]<0){
                        (*lianzhuptr)[x1][y1][dir_4]=lianzhu_calc_map[pattern_neg_black][false][BLACK];
                        (*boardptr)[x1][y1]=lianzhu_judge_ban(x1,y1,par_k)?BAN:EMPTY;
                    }

                    value[x1][y1][10-WHITE]-=value[x1][y1][dir_4+4*WHITE];
                    value[x1][y1][10-BLACK]-=value[x1][y1][dir_4+4*BLACK];
                    value[x1][y1][dir_4+4*WHITE]=fg4_value_map[lianzhu_calc_map[pattern_neg_white&((-1)^(0b11<<8))|(WHITE<<8)][false/*ban_black&&WHITE*/][WHITE]];
                    value[x1][y1][dir_4+4*BLACK]=fg4_value_map[lianzhu_calc_map[pattern_neg_black&((-1)^(0b11<<8))|(BLACK<<8)][false/*ban_black&&BLACK*/][BLACK]];
                    value[x1][y1][10-WHITE]+=value[x1][y1][dir_4+4*WHITE];
                    value[x1][y1][10-BLACK]+=value[x1][y1][dir_4+4*BLACK];
                    value[x1][y1][0]=value[x1][y1][10-WHITE]+
                        ((ban_black&&board[x1][y1]==BAN)?0:value[x1][y1][10-BLACK]);
                    
                }else{
                    break;
                }
            }
        //}while(color^=1);
    }
}
