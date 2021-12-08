#include "wuziqi.h"
#include "lianzhu2.h"
//#include "robot12.h"
//#include "robot3.h"
#include "robot4.h"
#include "robot5.h"

bool win_or_not(int x0,int y0,bool whom){
    int i=1,j=1;
    while(i+j!=6&&(x0+i<=15&&board[x0+i][y0]>0&&board[x0+i][y0]%2==whom&&++i||x0-j>=1&&board[x0-j][y0]>0&&board[x0-j][y0]%2==whom&&++j));
    if(i+j==6) return true;
    i=1;j=1;
    while(i+j!=6&&(y0+i<=15&&board[x0][y0+i]>0&&board[x0][y0+i]%2==whom&&++i||y0-j>=1&&board[x0][y0-j]>0&&board[x0][y0-j]%2==whom&&++j));
    if(i+j==6) return true;
    i=1;j=1;
    while(i+j!=6&&(x0+i<=15&&y0+i<=15&&board[x0+i][y0+i]>0&&board[x0+i][y0+i]%2==whom&&++i||x0-j>=1&&y0-j>=1&&board[x0-j][y0-j]>0&&board[x0-j][y0-j]%2==whom&&++j));
    if(i+j==6) return true;
    i=1;j=1;
    while(i+j!=6&&(x0+i<=15&&y0-i>=1&&board[x0+i][y0-i]>0&&board[x0+i][y0-i]%2==whom&&++i||x0-j>=1&&y0+j<=15&&board[x0-j][y0+j]>0&&board[x0-j][y0+j]%2==whom&&++j));
    if(i+j==6) return true;
    return false;
}

void choose_player(){
    char ch;
    do{
        printf("�ڷ�Ϊ��\n0.����\n1.����\n");
        ch=getchar();
        for(char tmp;(tmp=getchar())!='\n'&&tmp!=EOF;);//��ջ�����
    }while(ch!='0'&&ch!='1');
    printf("��ѡ��ڷ�Ϊ");
    printf((is_robot[black]=ch-'0')?"����\n":"����\n");
    printf("\n");
    do{
        printf("�׷�Ϊ��\n0.����\n1.����\n");
        ch=getchar();
        for(char tmp;(tmp=getchar())!='\n'&&tmp!=EOF;);
    }while(ch!='0'&&ch!='1');
    printf("��ѡ��׷�Ϊ");
    printf((is_robot[white]=ch-'0')?"����\n":"����\n");
    printf("\n");
    do{
        printf("�����֣�\n0.����\n1.��\n");
        ch=getchar();
        for(char tmp;(tmp=getchar())!='\n'&&tmp!=EOF;);
    }while(ch!='0'&&ch!='1');
    printf("��");
    printf((ban_black=ch-'0')?"��ֹ����\n":"��������\n");
    printf("\n");
    printf("ʹ����ĸ+��������\n");
    printf("ʹ��re/RE/rE/Re����\n");
    printf("��ս������ʼ�������������\n");
    getchar();
}
void initialize(){
    srand((unsigned)time(NULL));
    memset(board,0,sizeof(board));
    memset(timeline,0,sizeof(timeline));
    step=0;
    err=0;
    black_robot.nt();
    white_robot.nt();
}
void print_char(int i,int j){//https://zh.wikipedia.org/wiki/%E6%96%B9%E6%A1%86%E7%BB%98%E5%88%B6%E5%AD%97%E7%AC%A6
    if(board[i][j]==0){
        if(i==15){
            printf(j==1?"��":j==15?"��":"��");
        }else if(i==1){
            printf(j==1?"��":j==15?"��":"��");
        }else{
            printf(j==1?"��":j==15?"��":"��");
        }
        //if (j != 15) printf(" ");//��ʹ���°����̨��ȡ��ע����һ��
    }else if(board[i][j]==-2){
        printf("��");
    }else if(board[i][j]%2){//black
        printf(board[i][j]==step-1?"��":"��");
    }else{//white
        printf(board[i][j]==step-1?"��":"��");
    }
#ifdef __linux__
    if(j!=15) printf("��");
#endif
    return;
}
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
bool print_err(){
    switch (err){
    case 1:
        printf("������Υ������\n");
        break;
    case 2:
        printf("������Υ������\n");
        break;
    case 3:
        printf("��λ���Ѿ�����\n");
        break;
    case 4:
        printf("��һ�ӵ�����Ԫ\n");
        break;
    case 10:
        printf("�õ�Ϊ�ڷ�����\n");
        break;
    case 8:
        printf(step%2?"�׷�":"�ڷ�");
        printf("��ʤ\n\n");
        return false;
    case 9:
        printf("ƽ��\n\n");
        return false;
    while(true){
        case 5:
            printf("����������");
            break;
        case 6:
            printf("����һ����");
            break;
        case 7:
            printf("�޷����壬");
            break;
    }
    default:
        if(step!=1) printf("��һ��λ��Ϊ: %c%d\n",'A'+last_y-1,last_x);
        else printf("��һ��������Ԫ\n");
        break;
    }
    err=0;
    return true;
}
void retract(){
    if(--step){
        int prev_x=timeline[step].x;
        int prev_y=timeline[step].y;
        board[prev_x][prev_y]=0;
        if(--step){
            prev_x=timeline[step].x;
            prev_y=timeline[step].y;
            board[prev_x][prev_y]=0;
            err=5;
        }else{
            err=6;
            step++;
        }
    }else{
            err=7;
            step++;
    }
    if(--step){
        last_x=timeline[step].x;
        last_y=timeline[step].y;
    }
}
void announce_retract(){
    if(is_robot[black]) black_robot.re(black);
    if(is_robot[white]) white_robot.re(white);
}

bool get_input(){
    if(is_robot[step%2]){
        step%2?black_robot.fg():white_robot.fg();
        if(!(x&&y)){
            err=9;
            return false;
        }
        return true;
    }else{
        //����
        printf("�������%d��\n",step);
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
        //�����ж�
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

bool debug_input(){
    step%2?black_robot.fg():white_robot.fg();
    //����
    printf("�������%d��\n",step);
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
    //�����ж�
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
        fg4_x_self=x;
        fg4_y_self=y;
        return true;
    }
}

bool judge_input(){//todo: ���֣�+ʤ�����ж�
    if(board[x][y]>0){
        err=3;
        step--;
        return false;
    }
    if(step==1&&(x!=8||y!=8)){
        err=4;
        step--;
        return false;
    }
    if(ban_black&&step&1&&board[x][y]==-2){
        err=10;
        step--;
        return false;
    }
    if(win_or_not(x,y,step&1)) err=8;
    return true;
}
void store_input(){
    board[x][y]=step;
    last_x=timeline[step].x=x;
    last_y=timeline[step].y=y;
}

void train(){
    choose_player();
    int n=20;
    while(1){
        int win1=0,win2=0;
        int win1r=0,win2r=0;
        for(int k=1;k<=n/2;k++){
            black_robot.fg=fg4;
            black_robot.re=re4;
            black_robot.nt=nt4;
            white_robot.fg=fg5;
            white_robot.re=re5;
            white_robot.nt=nt5;
            initialize();
            while(++step){
                clear();
                print_board();
                if(!print_err()) break;
                if(!(get_input()&&judge_input())) continue;
                store_input();
                //getchar();
            }
            if(step<=15*15){
                win1+=(step+1)%2;
                win2+=step%2;
            }
            if(step<20){
                win1+=(step+1)%2;
                win2+=step%2;
            }
            getchar();
        }
        for(int k=1;k<=n/2;k++){
            black_robot.fg=fg5;
            black_robot.re=re5;
            black_robot.nt=nt5;
            white_robot.fg=fg4;
            white_robot.re=re4;
            white_robot.nt=nt4;
            initialize();
            while(++step){
                clear();
                print_board();
                if(!print_err()) break;
                if(!(get_input()&&judge_input())) continue;
                store_input();
                //getchar();
            }
            if(step<=15*15){
                win2r+=(step+1)%2;
                win1r+=step%2;
            }
            if(step<20){
                win2r+=(step+1)%2;
                win1r+=step%2;
            }
            getchar();
        }
        printf("\n%d %d \n",win1,win2);
        printf("%d %d \n",win1r,win2r);
        getchar();
    }
}

int main(){
#ifndef __linux__
    system("color F0");
#endif
    //train();
/*
    black_robot.fg=fg1;
    black_robot.re=re12;
    black_robot.nt=nt12;
    white_robot.fg=fg4;
    white_robot.re=re4;
    white_robot.nt=nt4;
*//*
    black_robot.fg=fg4;
    black_robot.re=re4;
    black_robot.nt=nt4;
    white_robot.fg=fg1;
    white_robot.re=re12;
    white_robot.nt=nt12;
*/
    black_robot.fg=fg5;
    black_robot.re=re5;
    black_robot.nt=nt5;
    white_robot.fg=fg5;
    white_robot.re=re5;
    white_robot.nt=nt5;
    choose_player();
    initialize();
    while(++step){
        if(ban_black) lianzhu_refresh_ban();
        clear();
        print_board();
        if(!print_err()) break;
        if(!(get_input()&&judge_input())) continue;
        store_input();
        //getchar();
        sleep(1);
    }
    printf("����EOF�˳�");
    while (getchar() != EOF);
}