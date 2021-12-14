#include "wuziqi.h"

int fg4_calc_value(int x0,int y0,bool color,int dir_4){

    int rem=board[x0][y0];
    board[x0][y0]=color;
    //if(ban_black) lianzhu_refresh_ban_at_dir_near(x0,y0,dir_4);

    int score;
    switch (lianzhu_calc(x0,y0,color,dir_4,ban_black)){
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
    //if(ban_black) lianzhu_refresh_ban_at_dir_near(x0,y0,dir_4); 

    return score;
}

int fg4_value_map(int pattern){
    switch (pattern){
    case CHANG_LIAN: return 5000000;break;
    case CHENG_5: return 5000000;break;
    case HUO_4: return 100000;break;
    case CHONG_4: return 10000;break;
    case SI_4: return 0;break;
    //case _3_4:
    case _4_4: return 20000;break;
    case HUO_3: return 8000;break;
    case TIAO_HUO_3: return 7000;break;
    case MIAN_3: return 500;break;
    case HUO_2: return 50;break;
    case MIAN_2: return 10;break;
    default: return 0;break;
    }
}

void fg4_refresh_value(int (*value)[16][11],int x0,int y0){
    if(ban_black) lianzhu_refresh_ban_near(x0,y0);
    //DONE: 211127 
    //注：211112的那版程序这里有问题，这句是必须的。
    //原因在于robot5调用robot4时，若每层落子点附近的禁手点不刷新，其效果会累积。
    //robot4自身使用时没这个问题，因为棋盘每轮稳定刷新禁手点。
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
                if(board[x3][y3]==EDGE){
                    pattern_pos_white|=(WHITE^1);
                    pattern_pos_black|=(BLACK^1);
                    continue;
                }
                pattern_pos_white|=(board[x3][y3]&0b11);
                pattern_pos_black|=(board[x3][y3]&0b11);
            }
            int pattern_neg_white=pattern_pos_white;
            int pattern_neg_black=pattern_pos_black;


            int x1=x0;
            int y1=y0;
            for(int k=1;k<=4;k++){
                x1+=dx;
                y1+=dy;
                if(board[x1][y1]!=EDGE){
                    pattern_pos_white<<=2;
                    pattern_pos_black<<=2;
                    pattern_pos_white&=((1<<18)-1);
                    pattern_pos_black&=((1<<18)-1);
                    if(board[x1+4*dx][y1+4*dy]==EDGE){
                        pattern_pos_white|=(WHITE^1);
                        pattern_pos_black|=(BLACK^1);
                    }else{
                        pattern_pos_black|=(board[x1+4*dx][y1+4*dy]&0b11);
                        pattern_pos_white|=(board[x1+4*dx][y1+4*dy]&0b11);
                    }
                    
                    value[x1][y1][10-WHITE]-=value[x1][y1][dir_4+4*WHITE];
                    value[x1][y1][10-BLACK]-=value[x1][y1][dir_4+4*BLACK];
                    value[x1][y1][dir_4+4*WHITE]=fg4_value_map(lianzhu_calc_map[pattern_pos_white&((-1)^(0b11<<8))|(WHITE<<8)][ban_black&&WHITE][WHITE]);
                    value[x1][y1][dir_4+4*BLACK]=fg4_value_map(lianzhu_calc_map[pattern_pos_black&((-1)^(0b11<<8))|(BLACK<<8)][ban_black&&BLACK][BLACK]);
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
                if(board[x1][y1]!=EDGE){
                    pattern_neg_white>>=2;
                    pattern_neg_black>>=2;
                    if(board[x1-4*dx][y1-4*dy]==EDGE){
                        pattern_neg_white|=((WHITE^1)<<16);
                        pattern_neg_black|=((BLACK^1)<<16);
                    }else{
                        pattern_neg_white|=((board[x1-4*dx][y1-4*dy]&0b11)<<16);
                        pattern_neg_black|=((board[x1-4*dx][y1-4*dy]&0b11)<<16);
                    }
                    
                    value[x1][y1][10-WHITE]-=value[x1][y1][dir_4+4*WHITE];
                    value[x1][y1][10-BLACK]-=value[x1][y1][dir_4+4*BLACK];
                    value[x1][y1][dir_4+4*WHITE]=fg4_value_map(lianzhu_calc_map[pattern_neg_white&((-1)^(0b11<<8))|(WHITE<<8)][ban_black&&WHITE][WHITE]);
                    value[x1][y1][dir_4+4*BLACK]=fg4_value_map(lianzhu_calc_map[pattern_neg_black&((-1)^(0b11<<8))|(BLACK<<8)][ban_black&&BLACK][BLACK]);
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
                    if(board[i][j]<0&&fg4_value[i][j][0]>=max){
                        //if(max==fg4_value[i][j][0]&&rand()%4){
                        //    continue;
                        //}
                        x=i;
                        y=j;
                        max=fg4_value[i][j][0];
                    }
                }
            }/*
            for(int i=1;i<=15;i++){
                for(int j=1;j<=15;j++){
                    if(board[i][j]==EMPTY&&win_or_not(i,j,!(step&1))){
                        x=i;
                        y=j;
                    }
                }
            }
            for(int i=1;i<=15;i++){
                for(int j=1;j<=15;j++){
                    if(board[i][j]<0&&win_or_not(i,j,step&1)){
                        x=i;
                        y=j;
                    }
                }
            }*/
        }else{
            for(int i=1;i<=15;i++){
                for(int j=1;j<=15;j++){
                    if(board[i][j]==EMPTY&&fg4_value[i][j][0]>=max){
                        //if(max==fg4_value[i][j][0]&&rand()%4){
                        //    continue;
                        //}
                        x=i;
                        y=j;
                        max=fg4_value[i][j][0];
                    }
                }
            }/*
            for(int i=1;i<=15;i++){
                for(int j=1;j<=15;j++){
                    if(board[i][j]==EMPTY&&win_or_not(i,j,!(step&1))){
                        x=i;
                        y=j;
                    }
                }
            }
            for(int i=1;i<=15;i++){
                for(int j=1;j<=15;j++){
                    if(board[i][j]==EMPTY&&win_or_not(i,j,step&1)){
                        x=i;
                        y=j;
                    }
                }
            }*/
        }
    }else{
        x=y=8;
    }
    fg4_x_self=x;
    fg4_y_self=y;
}

void nt4(){
    memset(fg4_value,0,sizeof(fg4_value));
    lianzhu_calc_init();
}

void re4(bool is_black){
    for(int j=7-err;j>0;j--){
        int tmp_x=timeline[step+j].x;
        int tmp_y=timeline[step+j].y;
        fg4_refresh_value(fg4_value,tmp_x,tmp_y);
    }
}

