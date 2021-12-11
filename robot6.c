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
    bool whom=!(is_self^fg5_self);
    if(win_or_not(node->i,node->j,whom)){
        node->value=is_self?1000000000:-1000000000;
        return;
    }
    int rem=board[node->i][node->j];
    board[node->i][node->j]=whom+2;
    fg4_refresh_value(fg5_value,node->i,node->j);

    //int i_new=node->i,j_new=node->j;
    //int score=fg6_minmax(alpha,beta,&i_new,&j_new,depth-1,!is_self);
    int score=fg6_minmax(alpha,beta,node,depth-1,!is_self);

    board[node->i][node->j]=rem;
    fg4_refresh_value(fg5_value,node->i,node->j);
    node->value=score;
}

void fg6_search_tops(bool whom,tree *node){
    for(int k=0;k<MAX_WIDTH;k++){
        node->son[k]=(tree *)malloc(sizeof(tree));
        memset(node->son[k],0,sizeof(tree));
    }
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
                if(curr_value>node->son[MAX_WIDTH-1]->value){
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
    case 6:
        if((double)(clock()-start)/CLOCKS_PER_SEC>13){
            printf("头秃(￣へ￣)\n");
            WIDTH=5;
        }
        break;
    case 5:
        break;
    default:
        if((double)(clock()-start)/CLOCKS_PER_SEC>9){
            printf("你好厉害(￣へ￣)\n");
            WIDTH=6;
        }
        break;
    }
    //DONE: adapt the deepest layer
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
                        //*x0=i;
                        //*y0=j;
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


    if(!node->searched) fg6_search_tops(!(is_self^fg5_self),node);
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
                    fg5_value[node->son[k]->i][node->son[k]->j][0]);
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
    fg6_tree=tree_choose(fg6_tree,fg5_x_self,fg5_y_self);
    fg6_tree=tree_choose(fg6_tree,last_x,last_y);
    if(fg6_minmax(-1000000000,+1000000000,fg6_tree,DEPTH,true)==1000000000)
        printf("嘿嘿，你要输啦！（￣︶￣）↗　\n");
}

void fg6(){
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
        fg6_calc(DEPTH);
        end=clock();
        printf("计算结束，用时%f\n",(double)(end-start)/CLOCKS_PER_SEC);
        /*if(!x||!y){//使用fg4（这样不好，因为对手长连不算输）
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
        }*/
    }
    fg5_x_self=x;
    fg5_y_self=y;
}

void nt6(){
    memset(fg5_value,0,sizeof(fg5_value));
    lianzhu_calc_init();
    fg6_tree=(tree *)malloc(sizeof(tree));
    memset(fg6_tree,0,sizeof(tree));
}

void re6(bool is_black){
    for(int j=7-err;j>0;j--){
        int tmp_x=timeline[step+j].x;
        int tmp_y=timeline[step+j].y;
        fg4_refresh_value(fg5_value,tmp_x,tmp_y);
    }
    tree_free(&fg6_tree);
    fg6_tree=(tree *)malloc(sizeof(tree));
    memset(fg6_tree,0,sizeof(tree));
}
