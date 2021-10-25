# 第二代五子棋人机对战程序

# 编译

## linux/gcc
编译参数记得加上

​		"-lm", //调用了math.h中的pow函数

​        "-finput-charset=GBK" //文件编码格式为GBK

## windows/msvc
需使用旧版控制台，可在命令提示符-属性-选项处设置

# 教程

## 规则

先手禁手，禁双三、双四、长连。

禁手与连五同时达成时，获胜为先。

黑子第一子必下天元。

## 基础功能

### 棋手

先手为黑，后手为白。为了方便逻辑判断，将两种颜色宏定义到1和0。

```c
#define black 1
#define white 0
```

### 棋盘

#### 棋盘的存储

棋盘通过一个全局数组（board）存储，默认设空，值为0。每下一子，在数组的对应位置记录当前落子步数（step）。并生成黑方禁手点，记录值为-2。

因此，通过判断数组值的正负性可以确定对应点是否为空，通过判断奇偶性可以确定落子方的颜色。

```c
int step;
int board[16][16];
```

#### 棋盘的打印

制表字符可以参考[方框绘制字符 - 维基百科](https://zh.wikipedia.org/wiki/%E6%96%B9%E6%A1%86%E7%BB%98%E5%88%B6%E5%AD%97%E7%AC%A6)。需要额外注意的是：不同终端下输出的字符宽度不同，需要根据特定的编译环境指定对应的字符，例如使用：

```c
void print_char(int i,int j){
    if(board[i][j]==0){
        if(i==15){
            printf(j==1?"┌":j==15?"┐":"┬");
        }else if(i==1){
            printf(j==1?"└":j==15?"┘":"┴");
        }else{
            printf(j==1?"├":j==15?"┤":"┼");
        }
    }else if(board[i][j]==-2){
        printf("×");
    }else if(board[i][j]%2){//black
        printf(board[i][j]==step-1?"▲":"●");
    }else{//white
        printf(board[i][j]==step-1?"△":"○");
    }
#ifdef __linux__
    if(j!=15) printf("─");
#endif
    return;
}
```

``` c
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
}
```

### 输入

使用函数（get_input）输入到全局变量（x，y），具体实现略。

``` c
int x,y;
bool get_input();
```

需要注意的是，输入有可能不合法，此时需要引入错误判断（judge_input）。如果没有错误，则存储落子时间到对应的落子点（store_input），并更新时间线（如果需要悔棋功能的话）。如果有错误，更新错误变量（err）为当前错误代码，并输出（print_err）。

```c
int err;
bool judege_input();
bool print_err(){
    switch (err){
    case 1:printf("横坐标违法输入\n");break;
    case 2:printf("纵坐标违法输入\n");break;
    case 3:printf("该位置已经有子\n");break;
    case 4:printf("第一子当落天元\n");break;
    case 10:printf("该点为黑方禁手\n");break;
    case 8:
        printf(step%2?"白方":"黑方");
        printf("获胜\n\n");
        return false;
    case 9:printf("平局\n\n");return false;
    while(true){
        case 5:printf("悔棋两步，");break;
        case 6:printf("悔棋一步，");break;
        case 7:printf("无法悔棋，");break;
    }
    default:
        if(step!=1) printf("上一步位置为: %c%d\n",'A'+last_y-1,last_x);
        else printf("第一步请落天元\n");
        break;
    }
    err=0;
    return true;
}
```

### 胜负判断

每次落点，遍历该点的四个方向。对每个方向，从落子点向两端延伸，直至遇到非同色点或成五。需要额外注意的是延伸范围不能超过棋盘边缘。

```c
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
```

### 初始化

在游戏开始前，调用函数（choose_player）来选定双方玩家以及是否开启禁手功能。然后调用函数（initialize）对数组等变量初始化。

```c
void choose_player();
void initialize();
```

### 流程

一个完整的实现可以表示如下：

```c
int main(){
#ifndef __linux__
    system("color F0");
#endif
    //暂时忽略这一段
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
    getchar();
}
```

为了兼容不同平台，宏定义sleep()与clear()函数

```c
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#include <conio.h>
#include <windows.h>
#define sleep(a) Sleep(1000*(a))
#define clear() system("cls")
#elif __linux__
#include <unistd.h>
#define clear() system("clear")
#endif
```

## 悔棋

### 触发

在玩家输入函数（get_input）处完成判断。当输入"re","rE","Re","RE"之一时触发悔棋。

```c
bool get_input(){
    ...
        if(!(strcmp(input,"re")&&strcmp(input,"Re")&&strcmp(input,"RE")&&strcmp(input,"rE"))){
			...
            return false;
        }
    ...
}
```

### 实现

悔棋主要通过维护一个落子的时间线数组（timeline）实现。

``` c
typedef struct point{
    int x,y;
}point;
point timeline[15*15+2];
```

每次悔棋（retract）时，从时间线中读取之前一到两个值，并悔棋一到两步（两步是因为人机对战时，单悔棋一步会立刻被机器落子覆盖，导致无法成功悔棋）。

```c
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
```

为便利后续落子机器悔棋，引入函数（announce_retract）通知机器玩家触发了悔棋。

```c
void announce_retract(){
    if(is_robot[black]) black_robot.re(black);
    if(is_robot[white]) white_robot.re(white);
}
```

## 禁手

### 禁手判断

## 机器

### 第一代

### 第一代改进版

### 第一点五代

### 第二代

### 第二代最终版

