#include "wuziqi.h"

tree *tree_choose(tree *node,int i,int j){
    tree *ans=NULL;
    for(int k=0;k<MAX_WIDTH&&node->son[k];k++){
        if(node->son[k]->i==i&&node->son[k]->j==j){
            ans=node->son[k];
            k++;
            printf("预料之内(～￣▽￣)～\n");
            for(;k<MAX_WIDTH&&node->son[k];k++){
                tree_free(&node->son[k]);
            }
            break;
        }else{
            tree_free(&node->son[k]);
            printf("意料之外(°Д°)\n");
        }
    }
    free(node);
    if(!ans){
        ans=(tree *)malloc(sizeof(tree));
        memset(ans,0,sizeof(tree));
    }
    return ans;
}

tree *tree_init(int value,int i,int j){
    tree *ans=(tree *)malloc(sizeof(tree));
    ans->value=value;
    ans->i=i;
    ans->j=j;
    ans->searched=false;
    memset(ans->son,0,sizeof(ans->son));
    return ans;
}

void tree_free(tree **nodeptr){
    if((*nodeptr)->searched){
        for(int k=0;k<MAX_WIDTH&&(*nodeptr)->son[k];k++)
            tree_free(&((*nodeptr)->son[k]));
    }
    free(*nodeptr);
    (*nodeptr)=NULL;
}

void fg6_calc_score(int alpha,int beta,tree *node,int depth,bool is_self){
    bool whom=!(is_self^fg6_self);
    if(win_or_not(node->i,node->j,whom)){
        node->value=is_self?1000000000:-1000000000;
        return;
    }
    int rem=board[node->i][node->j];
    board[node->i][node->j]=whom;
    fg4_refresh_value(fg6_value,node->i,node->j);

    int score=fg6_minmax(alpha,beta,node,depth-1,!is_self);

    board[node->i][node->j]=rem;
    fg4_refresh_value(fg6_value,node->i,node->j);
    node->value=score;
}
/*
bool fg6_ban_lianzhu_find(int *x0,int *y0,int dx,int dy,int *n,bool *blank,int num){
    while((board[*x0][*y0]==EDGE&&*y0>=1||(*blank=false))&&*n<=4){
        if(board[*x0][*y0]<0){
            //为空
            break;
        }else if((board[*x0][*y0]&1)!=BLACK){
            //敌方
            *blank=false;
            break;
        }else{
            //己方
            for(int dir_4=1;dir_4<=4;dir_4++){
                switch(fg6_value[*x0][*y0][dir_4+4*BLACK]){
                    case 8000:case 7000:
                    if(num==3) return true;
                    break;
                    case 100000:case 10000:
                    if(num==4) return true;
                    break;
                    default:break;
                }
            }
        }
        *x0+=dx;
        *y0+=dy;
        (*n)++;
    }
    return false;
}

bool fg6_judge_complex_ban(int x0,int y0,int dir,int num){
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
    if(fg6_ban_lianzhu_find(&i_x,&i_y,dx,dy,&i,&i_blank,num)) return true;
    if(fg6_ban_lianzhu_find(&j_x,&j_y,-dx,-dy,&j,&j_blank,num)) return true;
    int ii_x=i_x+dx,ii_y=i_y+dy,ii=i+1;
    bool ii_blank=i_blank;
    int jj_x=j_x-dx,jj_y=j_y-dy,jj=j+1;
    bool jj_blank=j_blank;
    if(i_blank){
        if(fg6_ban_lianzhu_find(&ii_x,&ii_y,dx,dy,&ii,&ii_blank,num)) return true;
    }
    if(j_blank){
        if(fg6_ban_lianzhu_find(&jj_x,&jj_y,-dx,-dy,&jj,&jj_blank,num)) return true;
    }
    return false;
}
*/

void fg6_search_tops(bool banned,tree *node){
    for(int k=0;k<MAX_WIDTH;k++){
        node->son[k]=(tree *)malloc(sizeof(tree));
        memset(node->son[k],0,sizeof(tree));
    }
    for(int i=1;i<=15;i++){
        for(int j=1;j<=15;j++){
            if(banned?board[i][j]==EMPTY:(board[i][j]<0)){
                int curr_value=fg6_value[i][j][0];
                if(curr_value>node->son[MAX_WIDTH-1]->value){
                    /*
                    //也许还有bug
                    if(banned){
                        bool cnt=false;
                        bool force_ncnt=false;
                        for(int dir_4=1;dir_4<=4;dir_4++){
                            int type=fg4_calc_value(i,j,BLACK,dir_4);
                            switch(type){
                            case 8000:case 7000:
                                if(fg6_judge_complex_ban(i,j,dir_4,3)){
                                    //printf("奇怪的禁手增加了\n");
                                    //getchar();
                                    cnt=true;
                                }
                                break;
                            case 100000:case 10000:
                                if(fg6_judge_complex_ban(i,j,dir_4,4)){
                                    //printf("奇怪的禁手增加了\n");
                                    //getchar();
                                    cnt=true;
                                }
                                break;
                            case 5000000:
                                force_ncnt=true;
                                break;
                            default:
                                break;
                            }
                        }
                        if(!force_ncnt&&cnt){
                            continue;
                        }
                    }
                    */

                    int k=MAX_WIDTH-2;
                    for(;k>=0;k--){
                        if(curr_value>node->son[k]->value){
                            node->son[k+1]->value=node->son[k]->value;
                            node->son[k+1]->i=node->son[k]->i;
                            node->son[k+1]->j=node->son[k]->j;
                        }else break;
                    }
                    node->son[k+1]->value=curr_value;
                    node->son[k+1]->i=i;
                    node->son[k+1]->j=j;
                }
            }
        }
    }
    node->searched=true;
}

int fg6_minmax(int alpha,int beta,tree *node,int depth,bool is_self){
    switch(WIDTH){
    case 10:
        if((double)(clock()-start)/CLOCKS_PER_SEC>13){
            printf("头秃(￣へ￣)\n");
            WIDTH=8;
        }
        break;
    case 8:
        break;
    default:
        if((double)(clock()-start)/CLOCKS_PER_SEC>10){
            printf("你好厉害(￣へ￣)\n");
            WIDTH=10;
        }
        break;
    }
    //DONE: adapt the deepest layer
    if(!depth){
        //也许还有bug
        if(fg6_self){
            me_max=thee_max=0;
            for(int i=1;i<=15;i++){
                for(int j=1;j<=15;j++){
                    if(board[i][j]>=0){//按场上现有的子来评分，判断那子的落点能否与同色子连珠
                        if(board[i][j]==BLACK){
                            if(me_max<=fg6_value[i][j][10-BLACK]) me_max=fg6_value[i][j][10-BLACK];
                        }
                    }else{
                        //if(fg6_value[i][j][10-!fg6_self]) printf("%d\n",fg6_value[i][j][10-!fg6_self]);
                        if(thee_max<=fg6_value[i][j][10-WHITE]){
                            //*x0=i;
                            //*y0=j;
                            thee_max=fg6_value[i][j][10-WHITE];
                        }
                    }
                }
            }
            return fg6_self==charge?me_max-thee_max:me_max-2*thee_max;
        }else{
            deepest_value=0;
            for(int i=1;i<=15;i++){
                for(int j=1;j<=15;j++){
                    if(board[i][j]==EMPTY){
                        deepest_value-=fg6_value[i][j][10-!fg6_self];
                    }else if(board[i][j]==fg6_self){
                        deepest_value+=fg6_value[i][j][10-fg6_self];
                    }
                }
            }
            return deepest_value;
        }
    }

    if(!node->searched) fg6_search_tops(!(is_self^fg6_self),node);
    /*else{
        printf("在第%d层，%c%d被跳过\n",depth,'A'+node->j-1,node->i);
    //    getchar();
    }*/

    int EXM=is_self?-1000000000:1000000000,K=0;
    for(int k=0;k<WIDTH;k++){
        if(node->son[k]->i&&node->son[k]->j){
            fg6_calc_score(alpha,beta,node->son[k],depth,is_self);
            if(is_self){
                if(node->son[k]->value>EXM){
                    EXM=node->son[k]->value;
                    if(depth==DEPTH) K=k;
                }
#ifdef __linux__
                alpha = my_max(alpha, EXM);
#else
                alpha = max(alpha, EXM);
#endif
            }else{
                if(node->son[k]->value<EXM){
                    EXM=node->son[k]->value;
                    if(depth==DEPTH) K=k;
                }
#ifdef __linux__
                beta = my_min(beta, EXM);
#else
                beta = min(beta, EXM);
#endif
            }
            if(depth==DEPTH){
                printf("%c%d %d %d\n",node->son[k]->j+'A'-1,node->son[k]->i,node->son[k]->value,
                    fg6_value[node->son[k]->i][node->son[k]->j][0]);
            }
            if(beta<=alpha) break;
        }
    }
    if(depth==DEPTH){
        x=fg6_tree->son[K]->i;
        y=fg6_tree->son[K]->j;
    }
    return EXM;//fg6_tree->son[K]->value;
}

void fg6_calc(int depth){
    if(charge) printf("进攻！！\n");
    else printf("防御！！\n");
    fg6_tree=tree_choose(fg6_tree,fg6_x_self,fg6_y_self);
    fg6_tree=tree_choose(fg6_tree,last_x,last_y);
    int points=fg6_minmax(-1000000000,+1000000000,fg6_tree,DEPTH,true);
    if(points==1000000000) printf("嘿嘿，你要输啦！（￣︶￣）↗　\n");
    else if(points==-1000000000) printf("可以认输吗QAQ\n");
    if(!charge&&points>0) charge=1;
    else if(charge&&points<0) charge=0;
}

void fg6(){
    fg6_self=step&1;
    x=y=0;
    if(step<=2){
        charge=fg6_self;//TODO
        x=8;
        y=9-step;
    }else{
        fg4_refresh_value(fg6_value,fg6_x_self,fg6_y_self);
        //fg4_debug();
        fg4_refresh_value(fg6_value,last_x,last_y);
        //fg4_debug(fg6_value);
        printf("计算中...\n");
        start=clock();
        if(step<=BEGIN_STEP){
            WIDTH=BEGIN_WIDTH;
            DEPTH=BEGIN_DEPTH;//odd recommended
        }else{
            WIDTH=IDEAL_WIDTH;
            DEPTH=IDEAL_DEPTH;
        }
        fg6_calc(DEPTH);
        end=clock();
        printf("计算结束，用时%f\n",(double)(end-start)/CLOCKS_PER_SEC);
        if(!x||!y){//使用fg4
            printf("可以认输吗QAQ\n");
            int max=-1;
            if(!(step&1)){
                for(int i=1;i<=15;i++){
                    for(int j=1;j<=15;j++){
                        if(board[i][j]<0&&fg6_value[i][j][0]>=max){
                            if(max==fg6_value[i][j][0]&&rand()%4){
                                continue;
                            }
                            x=i;
                            y=j;
                            max=fg6_value[i][j][0];
                        }
                    }
                }
            }else{
                for(int i=1;i<=15;i++){
                    for(int j=1;j<=15;j++){
                        if(board[i][j]==EMPTY&&fg6_value[i][j][0]>=max){
                            if(max==fg6_value[i][j][0]&&rand()%4){
                                continue;
                            }
                            x=i;
                            y=j;
                            max=fg6_value[i][j][0];
                        }
                    }
                }
            }
        }
        /*//防止憨憨的机器干傻事（这样不好，因为对手长连不算输）
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
        }*/
    }
    fg6_x_self=x;
    fg6_y_self=y;
}

void nt6(){
    memset(fg6_value,0,sizeof(fg6_value));
    lianzhu_calc_init();
    fg6_tree=(tree *)malloc(sizeof(tree));
    memset(fg6_tree,0,sizeof(tree));
    fg4_value_map_init();
}

void re6(bool is_black){
    for(int j=7-err;j>0;j--){
        int tmp_x=timeline[step+j].x;
        int tmp_y=timeline[step+j].y;
        fg4_refresh_value(fg6_value,tmp_x,tmp_y);
    }
    tree_free(&fg6_tree);
    fg6_tree=(tree *)malloc(sizeof(tree));
    memset(fg6_tree,0,sizeof(tree));
}
