#pragma once
#include "wuziqi.h"
#include "lianzhu.h"

//第三维度0位存总评分，1-4位存黑棋评分，5-8存白棋评分，9存黑棋总分，10存白棋总分
int fg4_value[16][16][11];
int fg4_x_self,fg4_y_self;

int fg4_calc_value(int x0,int y0,bool color,int dir){

    int rem=board[x0][y0];
    board[x0][y0]=2+color;
    if(ban_black) lianzhu_refresh_ban_near(x0,y0);

    int score;
    switch (lianzhu_calc(x0,y0,color,dir,ban_black)){
    case CHANG_LIAN: score= 5000000;break;
    case CHENG_5: score= 5000000;break;
    case HUO_4: score= 100000;break;
    case CHONG_4: score= 10000;break;
    case SI_4: score= 0;break;
    //case _3_4:
    case _4_4: score= 20000;break;
    case HUO_3: score= 8000;break;
    case TIAO_HUO_3: score= 7000;break;
    case MIAN_3: score= 500;break;
    case HUO_2: score= 50;break;
    case MIAN_2: score= 10;break;
    default: score= 0;break;
    }

    board[x0][y0]=rem;
    if(ban_black) lianzhu_refresh_ban_near(x0,y0); 

    return score;
}

void fg4_refresh_value(int (*value)[16][11],int x0,int y0){
    for(int dir_8=1;dir_8<=8;dir_8++){
        int dx=0,dy=0,score=0;
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
        int x1,y1,dir_4;
        for(int k=1;k<=5;k++){
            x1=x0+k*dx;
            y1=y0+k*dy;
            if(x1>=1&&x1<=15&&y1>=1&&y1<=15){
                dir_4=(dir_8-1)%4+1;
                value[x1][y1][10-black]-=value[x1][y1][dir_4+4*black];
                value[x1][y1][10-white]-=value[x1][y1][dir_4+4*white];
                value[x1][y1][dir_4+4*black]=fg4_calc_value(x1,y1,black,dir_4);
                value[x1][y1][dir_4+4*white]=fg4_calc_value(x1,y1,white,dir_4);
                value[x1][y1][10-black]+=value[x1][y1][dir_4+4*black];
                value[x1][y1][10-white]+=value[x1][y1][dir_4+4*white];
                value[x1][y1][0]=value[x1][y1][10-white]+
                    (ban_black&&board[x1][y1]<0?0:value[x1][y1][10-black]);
            }else{
                break;
            }
        }
    }
}

void fg4_debug(int (*value)[16][11]){
    for(int i=1;i<=15;i++){
        for(int j=1;j<=15;j++){
            printf("%5d",value[16-i][j][0]);
        }
        printf("\n\n");
    }
}

void fg4(){
    int max=-1;
    x=y=0;
    if(step!=1){
        fg4_refresh_value(fg4_value,fg4_x_self,fg4_y_self);
        //fg4_debug();
        fg4_refresh_value(fg4_value,last_x,last_y);
        //fg4_debug(fg4_value);
        if(!(step&1)){
            for(int i=1;i<=15;i++){
                for(int j=1;j<=15;j++){
                    if(board[i][j]<=0&&fg4_value[i][j][0]>=max){
                        if(max==fg4_value[i][j][0]&&rand()%4){
                            continue;
                        }
                        x=i;
                        y=j;
                        max=fg4_value[i][j][0];
                    }
                }
            }
            for(int i=1;i<=15;i++){
                for(int j=1;j<=15;j++){
                    if(!board[i][j]&&win_or_not(i,j,!(step%2))){
                        x=i;
                        y=j;
                    }
                }
            }
            for(int i=1;i<=15;i++){
                for(int j=1;j<=15;j++){
                    if(board[i][j]<=0&&win_or_not(i,j,step%2)){
                        x=i;
                        y=j;
                    }
                }
            }
        }else{
            for(int i=1;i<=15;i++){
                for(int j=1;j<=15;j++){
                    if(!board[i][j]&&fg4_value[i][j][0]>=max){
                        if(max==fg4_value[i][j][0]&&rand()%4){
                            continue;
                        }
                        x=i;
                        y=j;
                        max=fg4_value[i][j][0];
                    }
                }
            }
            for(int i=1;i<=15;i++){
                for(int j=1;j<=15;j++){
                    if(!board[i][j]&&win_or_not(i,j,!(step%2))){
                        x=i;
                        y=j;
                    }
                }
            }
            for(int i=1;i<=15;i++){
                for(int j=1;j<=15;j++){
                    if(!board[i][j]&&win_or_not(i,j,step%2)){
                        x=i;
                        y=j;
                    }
                }
            }
        }
    }else{
        x=y=8;
    }
    fg4_x_self=x;
    fg4_y_self=y;
}

void nt4(){
    memset(fg4_value,0,sizeof(fg4_value));
}

void re4(bool is_black){
    for(int j=7-err;j>0;j--){
        int tmp_x=timeline[step+j].x;
        int tmp_y=timeline[step+j].y;
        fg4_refresh_value(fg4_value,tmp_x,tmp_y);
    }
}

