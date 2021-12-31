/**********************************************************************************************
Copyright (c) 2021 LazyBird
File name:      wuziqi.c
Description:    五子棋程序main函数所在源文件，完成棋盘落子、打印、判断等基础功能
Author:         Eric Jin
Date:           2021.12.31
**********************************************************************************************/
#include "wuziqi.h"

/*成五判断：在（x0,y0）点落下whom颜色的子，判断是否成五*/
bool win_or_not(int x0,int y0,bool whom){
    int i=1,j=1;
    while(i+j!=6&&(board[x0+i][y0]==whom&&++i||board[x0-j][y0]==whom&&++j));
    if(i+j==6) return true;
    i=1;j=1;
    while(i+j!=6&&(board[x0][y0+i]==whom&&++i||board[x0][y0-j]==whom&&++j));
    if(i+j==6) return true;
    i=1;j=1;
    while(i+j!=6&&(board[x0+i][y0+i]==whom&&++i||board[x0-j][y0-j]==whom&&++j));
    if(i+j==6) return true;
    i=1;j=1;
    while(i+j!=6&&(board[x0+i][y0-i]==whom&&++i||board[x0-j][y0+j]==whom&&++j));
    if(i+j==6) return true;
    return false;
}
/*开始界面玩家选择*/
void choose_player(){
    char ch;
    do{
        printf("黑方为：\n0.真人\n1.机器\n");
        ch=getchar();
        for(char tmp;(tmp=getchar())!='\n'&&tmp!=EOF;);//清空缓存区
    }while(ch!='0'&&ch!='1');
    printf("已选择黑方为");
    printf((is_robot[BLACK]=ch-'0')?"机器\n":"真人\n");
    printf("\n");
    do{
        printf("白方为：\n0.真人\n1.机器\n");
        ch=getchar();
        for(char tmp;(tmp=getchar())!='\n'&&tmp!=EOF;);
    }while(ch!='0'&&ch!='1');
    printf("已选择白方为");
    printf((is_robot[WHITE]=ch-'0')?"机器\n":"真人\n");
    printf("\n");
    do{
        printf("禁先手：\n0.不禁\n1.禁\n");
        ch=getchar();
        for(char tmp;(tmp=getchar())!='\n'&&tmp!=EOF;);
    }while(ch!='0'&&ch!='1');
    printf("已");
    printf((ban_black=ch-'0')?"禁止先手\n":"不禁先手\n");
    printf("\n");
    printf("使用字母+数字输入\n");
    printf("使用re/RE/rE/Re悔棋\n");
    printf("对战即将开始，按任意键进入\n");
    getchar();
}
/*初始化棋盘、时间线，并通知robot初始化*/
void initialize(){
    srand((unsigned)time(NULL));
    //棋盘初始化
    memset(board_expanded,EMPTY,sizeof(board_expanded));
    board=(int(*)[23])(board_expanded[3]+3);//BUG FIXED AT 211217
    for(int i=0;i<=22;i++){
        for(int j=0;j<=22;j++){
            if(i<4||i>18||j<4||j>18){
                board_expanded[i][j]=EDGE;
            }
        }
    }
    //时间线（用于悔棋）初始化
    memset(timeline,EMPTY,sizeof(timeline));
    step=0;
    //初始状态无错误
    err=0;
    //通知黑白二色的robot初始化
    black_robot.nt();
    white_robot.nt();
}
/*打印（i,j）点的棋盘字符*/
void print_char(int i,int j){
    //字符参考了https://zh.wikipedia.org/wiki/%E6%96%B9%E6%A1%86%E7%BB%98%E5%88%B6%E5%AD%97%E7%AC%A6
    if(board[i][j]==EMPTY){
        if(i==15){
            printf(j==1?"┌":j==15?"┐":"┬");
        }else if(i==1){
            printf(j==1?"└":j==15?"┘":"┴");
        }else{
            printf(j==1?"├":j==15?"┤":"┼");
        }
#ifndef __linux__
#ifdef NEW_CMD
        if (j != 15) printf(" ");//若使用新版控制台，取消注释这一行
#endif
#endif
    }else if(board[i][j]==BAN){
        printf("×");
    }else if(board[i][j]==BLACK){//black
        printf(i==last_x&&j==last_y?"▲":"●");
    }else{//white
        printf(i==last_x&&j==last_y?"△":"○");
    }
#ifdef __linux__
    if(j!=15) printf("─");
#endif
    return;
}
/*打印棋盘以及时间线*/
void print_board(){
    for(int i=15;i>=1;i--){
        printf("%2d",i);
        for(int j=1;j<=15;j++){
            print_char(i,j);
        }
        printf("\n");
    }
    printf(" ");
    for(int j=1;j<=15;j++){
        printf(" %c",'A'-1+j);
    }
    printf("\n");

    for(int i=1;i<step;i++){
        printf("%c%d ",timeline[i].y+'A'-1,timeline[i].x);
    }
    printf("\n");
}
/*根据err的错误代码打印错误信息，若游戏继续返回true，游戏结束返回false*/
bool print_err(){
    switch (err){
    case 1:
        printf("横坐标违法输入\n");
        break;
    case 2:
        printf("纵坐标违法输入\n");
        break;
    case 3:
        printf("该位置已经有子\n");
        break;
    case 4:
        printf("第一子当落天元\n");
        break;
    case 10:
        printf("该点为黑方禁手\n");
        break;
    case 8:
        printf(step%2?"白方":"黑方");
        printf("获胜\n\n");
        return false;
    case 9:
        printf("平局\n\n");
        return false;
    while(true){
        case 5:
            printf("悔棋两步，");
            break;
        case 6:
            printf("悔棋一步，");
            break;
        case 7:
            printf("无法悔棋，");
            break;
    }
    default:
        if(step!=1) printf("上一步位置为: %c%d\n",'A'+last_y-1,last_x);
        else printf("第一步请落天元\n");
        break;
    }
    err=0;
    return true;
}
/*悔棋*/
void retract(){
    if(--step){
        int prev_x=timeline[step].x;
        int prev_y=timeline[step].y;
        board[prev_x][prev_y]=EMPTY;
        if(--step){
            //悔棋两步
            prev_x=timeline[step].x;
            prev_y=timeline[step].y;
            board[prev_x][prev_y]=EMPTY;
            err=5;
        }else{
            //悔棋一步
            err=6;
            step++;
        }
    }else{
        //无法悔棋
        err=7;
        step++;
    }
    if(--step){
        //更新上一时刻的落子点
        last_x=timeline[step].x;
        last_y=timeline[step].y;
    }
}
/*通知robot悔棋*/
void announce_retract(){
    if(is_robot[BLACK]) black_robot.re(BLACK);
    if(is_robot[WHITE]) white_robot.re(WHITE);
}
/*获取输入，若输入合法返回true*/
bool get_input(){
    if(is_robot[step%2]){
        //当前玩家为robot，从robot计算函数获取落子点
        (step&1)?black_robot.fg():white_robot.fg();
        if(!(x&&y)){
            //无子可落，平局
            err=9;
            return false;
        }
        return true;
    }else{
        //当前玩家为human，从控制台读取落子点
        //输入
        printf("请输入第%d步\n",step);
        char input[4];
#ifdef __linux__
        scanf("%s",input);
#else
        scanf_s("%s",input,4);
#endif
        if(!(strcmp(input,"re")&&strcmp(input,"Re")&&strcmp(input,"RE")&&strcmp(input,"rE"))){
            retract();
            announce_retract();
            return false;
        }
        //错误判断
        if(!((y=input[0]-'A')>=0&&y<='O'-'A'||(y=input[0]-'a')>=0&&y<='o'-'a')){
            err=1;
            step--;
            return false;
        }else if(!((x=input[2]?input[2]-'0'+(input[1]-'0')*10:input[1]-'0')<=15&&x>=1)){
            err=2;
            step--;
            return false;
        }else{
            y++;
            return true;
        }
    }
}
/*判断输入是否合法，更新错误代码，若合法返回true，不合法返回false*/
bool judge_input(){
    if(board[x][y]>=0){
        //该位置已经有子
        err=3;
        step--;
        return false;
    }
    if(step==1&&(x!=8||y!=8)){
        //第一子当落天元
        err=4;
        step--;
        return false;
    }
    if(ban_black&&step&1&&board[x][y]==BAN){
        //该点为黑方禁手
        err=10;
        step--;
        return false;
    }
    if(win_or_not(x,y,step&1)) err=8; //获胜
    return true;
}
/*存储当前落子点到棋盘和时间线*/
void store_input(){
    board[x][y]=(step&1);
    last_x=timeline[step].x=x;
    last_y=timeline[step].y=y;
}

int main(){
    //棋盘反色
#ifndef __linux__
    system("color F0");
#endif

/* 老版本（1代、2代、3代（有bug）、4代、5代）的robot，已弃用，具体源文件见根目录下bak文件夹
 *   black_robot.fg=fg4;
 *   black_robot.re=re4;
 *   black_robot.nt=nt4;
 *   white_robot.fg=fg4;
 *   white_robot.re=re4;
 *   white_robot.nt=nt4;
 *   *//*
 *   black_robot.fg=fg4;
 *   black_robot.re=re4;
 *   black_robot.nt=nt4;
 *   white_robot.fg=fg1;
 *   white_robot.re=re12;
 *   white_robot.nt=nt12;
 */

    //使用最新版本（6代）robot
    black_robot.fg=fg6;
    black_robot.re=re6;
    black_robot.nt=nt6;
    white_robot.fg=fg6;
    white_robot.re=re6;
    white_robot.nt=nt6;

    choose_player();
    initialize();
    while(++step){
        if(ban_black) lianzhu_refresh_ban();
        clear();
        printf("作者：Eric Jin\n");
        print_board();
        if(!print_err()) break;
        if(!(get_input()&&judge_input())) continue;
        store_input();
        //getchar();
        sleep(1);
    }
    printf("输入EOF退出");
    while (getchar() != EOF);
}