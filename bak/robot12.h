#pragma once
#include "wuziqi.h"

int fg12_black_value[16][16][5];
int fg12_white_value[16][16][5];
void fg12_mark_value(int (*value)[16][5],int x0,int y0,bool plus){
    if(plus){
        for(int i=-4;i<=4;i++){
            x0+i<=15&&x0+i>=1&&(value[x0+i][y0][1]+=5-abs(i));
            y0+i<=15&&y0+i>=1&&(value[x0][y0+i][2]+=5-abs(i));
            x0+i<=15&&x0+i>=1&&y0+i<=15&&y0+i>=1&&(value[x0+i][y0+i][3]+=5-abs(i));
            x0+i<=15&&x0+i>=1&&y0-i<=15&&y0-i>=1&&(value[x0+i][y0-i][4]+=5-abs(i));
        }
    }else{
        for(int i=-4;i<=4;i++){
            x0+i<=15&&x0+i>=1&&(value[x0+i][y0][1]-=5-abs(i));
            y0+i<=15&&y0+i>=1&&(value[x0][y0+i][2]-=5-abs(i));
            x0+i<=15&&x0+i>=1&&y0+i<=15&&y0+i>=1&&(value[x0+i][y0+i][3]-=5-abs(i));
            x0+i<=15&&x0+i>=1&&y0-i<=15&&y0-i>=1&&(value[x0+i][y0-i][4]-=5-abs(i));
        }
    }
}
int fg1_calc_value(int (*value)[16][5],int *x0,int *y0){
    int max=-1;
    for(int i=1;i<=15;i++){
        for(int j=1;j<=15;j++){
            if((value[i][j][0]=
                (abs(value[i][j][1])>=9?abs(value[i][j][1])*4:abs(value[i][j][1])>=5?abs(value[i][j][1])*2:abs(value[i][j][1]))+
                (abs(value[i][j][2])>=9?abs(value[i][j][2])*4:abs(value[i][j][2])>=5?abs(value[i][j][2])*2:abs(value[i][j][2]))+
                (abs(value[i][j][3])>=9?abs(value[i][j][3])*4:abs(value[i][j][3])>=5?abs(value[i][j][3])*2:abs(value[i][j][3]))+
                (abs(value[i][j][4])>=9?abs(value[i][j][4])*4:abs(value[i][j][4])>=5?abs(value[i][j][4])*2:abs(value[i][j][4]))
            )>=max&&!board[i][j]){
                if(max==value[i][j][0]&&rand()%4){
                    continue;
                }
                *x0=i;
                *y0=j;
                max=value[i][j][0];
            }
        }
    }
    return max;
}
int fg2_calc_value(int (*value)[16][5],int *x0,int *y0){
    int max=-1;
    for(int i=1;i<=15;i++){
        for(int j=1;j<=15;j++){
            if((value[i][j][0]=100*(
                pow(abs(value[i][j][1]),2.3)+
                pow(abs(value[i][j][2]),2.3)+
                pow(abs(value[i][j][3]),2.3)+
                pow(abs(value[i][j][4]),2.3))
            )>=max&&!board[i][j]){
                if(max==value[i][j][0]&&rand()%4){
                    continue;
                }
                *x0=i;
                *y0=j;
                max=value[i][j][0];
            }
        }
    }
    return max;
}
void fg1(){
    int (*value)[16][5]=step%2?fg12_black_value:fg12_white_value;
    x=y=0;
    if(step!=1){
        fg12_mark_value(value,last_x,last_y,false);
        fg1_calc_value(value,&x,&y);
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
    }else{
        x=y=8;
    }
    fg12_mark_value(value,x,y,true);
}
void fg2(){
    int (*value)[16][5]=step%2?fg12_black_value:fg12_white_value;
    x=y=0;
    if(step!=1){
        fg12_mark_value(value,last_x,last_y,false);
        fg2_calc_value(value,&x,&y);
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
    }else{
        x=y=8;
    }
    fg12_mark_value(value,x,y,true);
}
void re12(bool is_black){
    int (*value)[16][5]=is_black?fg12_black_value:fg12_white_value;
    /*for(int i=15;i>=1;i--){
        for(int j=1;j<=15;j++){
            printf("%2d",value[i][j][1]);
        }
        printf("\n");
    }
    printf("err:%d\n",err);*/
    for(int j=7-err;j>0;j--){
        int tmp_x=timeline[step+j].x;
        int tmp_y=timeline[step+j].y;
        fg12_mark_value(value,tmp_x,tmp_y,(step+j)%2!=is_black);
    }
    /*for(int i=15;i>=1;i--){
        for(int j=1;j<=15;j++){
            printf("%2d",value[i][j][1]);
        }
        printf("\n");
    }
    printf("\n");
    getchar();
    getchar();*/
}
void nt12(){
    memset(fg12_black_value,0,sizeof(fg12_black_value));
    memset(fg12_white_value,0,sizeof(fg12_white_value));
}