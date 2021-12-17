#include "wuziqi.h"

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

void choose_player(){
    char ch;
    do{
        printf("�ڷ�Ϊ��\n0.����\n1.����\n");
        ch=getchar();
        for(char tmp;(tmp=getchar())!='\n'&&tmp!=EOF;);//��ջ�����
    }while(ch!='0'&&ch!='1');
    printf("��ѡ��ڷ�Ϊ");
    printf((is_robot[BLACK]=ch-'0')?"����\n":"����\n");
    printf("\n");
    do{
        printf("�׷�Ϊ��\n0.����\n1.����\n");
        ch=getchar();
        for(char tmp;(tmp=getchar())!='\n'&&tmp!=EOF;);
    }while(ch!='0'&&ch!='1');
    printf("��ѡ��׷�Ϊ");
    printf((is_robot[WHITE]=ch-'0')?"����\n":"����\n");
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
    memset(board_expanded,EMPTY,sizeof(board_expanded));
    board=(int(*)[23])(board_expanded[3]+3);//BUG FIXED AT 211217
    for(int i=0;i<=22;i++){
        for(int j=0;j<=22;j++){
            if(i<4||i>18||j<4||j>18){
                board_expanded[i][j]=EDGE;
            }
        }
    }
    memset(timeline,EMPTY,sizeof(timeline));
    step=0;
    err=0;
    black_robot.nt();
    white_robot.nt();
}

void print_char(int i,int j){//https://zh.wikipedia.org/wiki/%E6%96%B9%E6%A1%86%E7%BB%98%E5%88%B6%E5%AD%97%E7%AC%A6
    if(board[i][j]==EMPTY){
        if(i==15){
            printf(j==1?"��":j==15?"��":"��");
        }else if(i==1){
            printf(j==1?"��":j==15?"��":"��");
        }else{
            printf(j==1?"��":j==15?"��":"��");
        }
#ifndef __linux__
        if (j != 15) printf(" ");//��ʹ���°����̨��ȡ��ע����һ��
#endif
    }else if(board[i][j]==BAN){
        printf("��");
    }else if(board[i][j]==BLACK){//black
        printf(i==last_x&&j==last_y?"��":"��");
    }else{//white
        printf(i==last_x&&j==last_y?"��":"��");
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
        board[prev_x][prev_y]=EMPTY;
        if(--step){
            prev_x=timeline[step].x;
            prev_y=timeline[step].y;
            board[prev_x][prev_y]=EMPTY;
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
    if(is_robot[BLACK]) black_robot.re(BLACK);
    if(is_robot[WHITE]) white_robot.re(WHITE);
}

bool get_input(){
    if(is_robot[step%2]){
        (step&1)?black_robot.fg():white_robot.fg();
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

bool judge_input(){
    if(board[x][y]>=0){
        err=3;
        step--;
        return false;
    }
    if(step==1&&(x!=8||y!=8)){
        err=4;
        step--;
        return false;
    }
    if(ban_black&&step&1&&board[x][y]==BAN){
        err=10;
        step--;
        return false;
    }
    if(win_or_not(x,y,step&1)) err=8;
    return true;
}

void store_input(){
    board[x][y]=(step&1);
    last_x=timeline[step].x=x;
    last_y=timeline[step].y=y;
}

int main(){
#ifndef __linux__
    system("color F0");
#endif
/*
    black_robot.fg=fg4;
    black_robot.re=re4;
    black_robot.nt=nt4;
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