#include "robot5.h"
#include "wuziqi.h"
#include "robot4.h"
#include "lianzhu2.h"

int WIDTH=IDEAL_WIDTH;
int DEPTH=IDEAL_DEPTH;

int me_max=0;
int thee_max=0;

void swap(int *a,int *b){
    int t=*a;
    *a=*b;
    *b=t;
}

int fg5_calc_score(int alpha,int beta,int *x0,int *y0,int i,int j,int depth,bool is_self){
    int score=(is_self?-1000000000:1000000000);
    bool whom=!(is_self^fg5_self);
    if(win_or_not(i,j,whom)){
        *x0=i;
        *y0=j;
        //printf("%d %d %d %d %s\n",*x0,*y0,-score,depth,"win");
        return -score;
    }
    int rem=board[i][j];
    board[i][j]=whom+2;

    fg4_refresh_value(fg5_value,i,j);
    int tmp=score,i0=i,j0=j;
    score=(is_self?
        my_max(score,minmax(alpha,beta,&i,&j,depth-1,!is_self)):
        my_min(score,minmax(alpha,beta,&i,&j,depth-1,!is_self))
    );
    //score=(is_self?*my_max:*my_min)(score,minmax(alpha,beta,&i,&j,depth-1,!is_self));
    if(score!=tmp){
        *x0=i0;
        *y0=j0;
        //printf("%d %d %d %d %d %d\n",*x0,*y0,score,depth,me_max,thee_max);
    }
    board[i0][j0]=rem;
    fg4_refresh_value(fg5_value,i0,j0);
    return score;
}

int minmax(int alpha,int beta,int *x0,int *y0,int depth,bool is_self){
    switch(WIDTH){
    case 6:
        if((double)(clock()-start)/CLOCKS_PER_SEC>14){
            printf("头秃￣へ￣\n");
            WIDTH=5;
        }
        break;
    case 5:
        break;
    default:
        if((double)(clock()-start)/CLOCKS_PER_SEC>10){
            printf("你好厉害￣へ￣\n");
            WIDTH=6;
        }
        break;
    }

    if(!depth){
        me_max=thee_max=0;
        for(int i=1;i<=15;i++){
            for(int j=1;j<=15;j++){
                if(board[i][j]>0){//按场上现有的子来评分，判断那子的落点能否与同色子连珠
                    if((board[i][j]&1)==fg5_self){
                        if(me_max<=fg5_value[i][j][10-fg5_self]) me_max=fg5_value[i][j][10-fg5_self];
                    }
                }else{
                    //if(fg5_value[i][j][10-!fg5_self]) printf("%d\n",fg5_value[i][j][10-!fg5_self]);
                    if((fg5_self||!board[i][j])&&thee_max<=fg5_value[i][j][10-!fg5_self]){
                        *x0=i;
                        *y0=j;
                        thee_max=fg5_value[i][j][10-!fg5_self];
                    }
                }
                /*
                if(board[i][j]>0){//按场上现有的子来评分，判断那子的落点能否与同色子连珠
                    if((board[i][j]&1)==fg5_self){
                        if(me_max<=fg5_value[i][j][10-fg5_self]) me_max=fg5_value[i][j][10-fg5_self];
                    }
                    else{
                        if(thee_max<=fg5_value[i][j][10-!fg5_self]) thee_max=fg5_value[i][j][10-!fg5_self];
                    }
                }*/
            }
        }
        return me_max-thee_max;
    }
    bool whom=!(is_self^fg5_self);
    int max_[MAX_WIDTH];
    int i_[MAX_WIDTH];
    int j_[MAX_WIDTH];
    int x_[MAX_WIDTH];
    int y_[MAX_WIDTH];
    int score_[MAX_WIDTH];
    memset(max_,0,sizeof(max_));
    memset(i_,0,sizeof(i_));
    memset(j_,0,sizeof(j_));
    memset(x_,0,sizeof(x_));
    memset(y_,0,sizeof(y_));
    memset(score_,0,sizeof(score_));
    for(int i=1;i<=15;i++){
        for(int j=1;j<=15;j++){
            if(whom?!board[i][j]:(board[i][j]<=0)){
                int curr_value=fg5_value[i][j][0];
                /*
                int curr_i=i;
                int curr_j=j;
                for(int k=0;k<WIDTH;k++){
                    if(curr_value>max_[k]){
                        swap(i_+k,&curr_i);
                        swap(j_+k,&curr_j);
                        swap(max_+k,&curr_value);
                    }
                }*/
                if(curr_value>max_[MAX_WIDTH-1]){
                    int k=MAX_WIDTH-2;
                    for(;k>=0;k--){
                        if(curr_value>max_[k]){
                            max_[k+1]=max_[k];
                            i_[k+1]=i_[k];
                            j_[k+1]=j_[k];
                        }else break;
                    }
                    max_[k+1]=curr_value;
                    i_[k+1]=i;
                    j_[k+1]=j;
                }
            }
        }
    }
    int EXM=is_self?-1000000000:1000000000,K=0;
    for(int k=0;k<WIDTH;k++){
        if(i_[k]&&j_[k]){
            score_[k]=fg5_calc_score(alpha,beta,x_+k,y_+k,i_[k],j_[k],depth,is_self);
            if(is_self){
                if(score_[k]>EXM){
                    EXM=score_[k];
                    K=k;
                }
                alpha=my_max(alpha,EXM);
            }else{
                if(score_[k]<EXM){
                    EXM=score_[k];
                    K=k;
                }
                beta=my_min(beta,EXM);
            }
            if(depth==DEPTH) printf("%c%d %d %d\n",j_[k]+'A'-1,i_[k],score_[k],fg5_value[i_[k]][j_[k]][0]);
            if(beta<=alpha) break;
        }
    }
    *x0=x_[K];//Why not i_[k]? seems all right
    *y0=y_[K];
    return score_[K];
}

void fg5(){
    fg5_self=step&1;
    x=y=0;
    if(step<=2){
        x=8;
        y=9-step;
    }else{
        fg4_refresh_value(fg5_value,fg5_x_self,fg5_y_self);
        //fg4_debug();
        fg4_refresh_value(fg5_value,last_x,last_y);
        //fg4_debug(fg5_value);
        printf("计算中...\n");
        start=clock();
        if(step<=BEGIN_STEP){
            WIDTH=BEGIN_WIDTH;
            DEPTH=BEGIN_DEPTH;//odd recommended
        }else{
            WIDTH=IDEAL_WIDTH;
            DEPTH=IDEAL_DEPTH;
        }
        if(minmax(-1000000000,+1000000000,&x,&y,DEPTH,true)==1000000000) printf("嘿嘿，你要输啦！（￣︶￣）↗　\n");
        end=clock();
        printf("计算结束，用时%f\n",(double)(end-start)/CLOCKS_PER_SEC);
        if(!x||!y){//使用fg4
            printf("可以认输吗QAQ\n");
            int max=-1;
            if(!(step&1)){
                for(int i=1;i<=15;i++){
                    for(int j=1;j<=15;j++){
                        if(board[i][j]<=0&&fg5_value[i][j][0]>=max){
                            if(max==fg5_value[i][j][0]&&rand()%4){
                                continue;
                            }
                            x=i;
                            y=j;
                            max=fg5_value[i][j][0];
                        }
                    }
                }
                for(int i=1;i<=15;i++){
                    for(int j=1;j<=15;j++){
                        if(!board[i][j]&&win_or_not(i,j,!(step&1))){
                            x=i;
                            y=j;
                        }
                    }
                }
                for(int i=1;i<=15;i++){
                    for(int j=1;j<=15;j++){
                        if(board[i][j]<=0&&win_or_not(i,j,step&1)){
                            x=i;
                            y=j;
                        }
                    }
                }
            }else{
                for(int i=1;i<=15;i++){
                    for(int j=1;j<=15;j++){
                        if(!board[i][j]&&fg5_value[i][j][0]>=max){
                            if(max==fg5_value[i][j][0]&&rand()%4){
                                continue;
                            }
                            x=i;
                            y=j;
                            max=fg5_value[i][j][0];
                        }
                    }
                }
                for(int i=1;i<=15;i++){
                    for(int j=1;j<=15;j++){
                        if(!board[i][j]&&win_or_not(i,j,!(step&1))){
                            x=i;
                            y=j;
                        }
                    }
                }
                for(int i=1;i<=15;i++){
                    for(int j=1;j<=15;j++){
                        if(!board[i][j]&&win_or_not(i,j,step&1)){
                            x=i;
                            y=j;
                        }
                    }
                }
            }
        }
        //防止憨憨的机器干傻事
        for(int i=1;i<=15;i++){
            for(int j=1;j<=15;j++){
                if(!board[i][j]&&win_or_not(i,j,!(step&1))){
                    x=i;
                    y=j;
                }
            }
        }
        for(int i=1;i<=15;i++){
            for(int j=1;j<=15;j++){
                if(!board[i][j]&&win_or_not(i,j,step&1)){
                    x=i;
                    y=j;
                }
            }
        }
    }
    fg5_x_self=x;
    fg5_y_self=y;
}

void nt5(){
    memset(fg5_value,0,sizeof(fg5_value));
    lianzhu_calc_init();
}

void re5(bool is_black){
    for(int j=7-err;j>0;j--){
        int tmp_x=timeline[step+j].x;
        int tmp_y=timeline[step+j].y;
        fg4_refresh_value(fg5_value,tmp_x,tmp_y);
    }
}

