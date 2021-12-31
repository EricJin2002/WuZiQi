/**********************************************************************************************
Copyright (c) 2021 LazyBird
File name:      wuziqi.h
Description:    五子棋程序的头文件，声明了robot4.c robot6.c lianzhu2.c里的所有函数和wuziqi.c里的部分函数
Author:         Eric Jin
Date:           2021.12.31
**********************************************************************************************/
#ifndef __WUZIQI_H__
#define __WUZIQI_H__

/*如果使用新版控制台，请保留该宏定义，反之请注释*/
#define NEW_CMD

#pragma comment(linker, "/STACK:102400000,102400000")
#include "opt.h"//gcc优化头文件
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>

//wuziqi.c

/*根据操作系统选择相应的sleep与clear函数*/
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#include <conio.h>
#include <windows.h>
#define sleep(a) Sleep(1000*(a))
#define clear() system("cls")
#elif __linux__
#include <unistd.h>
#define clear() system("clear")
#endif

/*落子机器人*/
struct robot{
    void (*nt)();               //initialize：用于初始化机器人
    void (*fg)();               //figure：用于计算落子点
    void (*re)(bool is_black);  //retract：用于悔棋
} black_robot,white_robot;

/*使用4+15+4=23的存储方式：预留4个点，用于判断边界*/
int board_expanded[23][23];
/*棋盘：1~15为棋盘有效范围，-3~0或16~19为边界范围*/
int (*board)[23];

typedef struct point{
    int x,y;
}point;
/*时间线：记录每次落子的横纵坐标，用于悔棋*/
point timeline[15*15+2];

int step;           //记录当前步数
int x,y;            //当前落子点
int last_x,last_y;  //上一时刻对手落子点
int err;            //错误信息，正常为0
bool is_robot[2];   //记录黑白双方分别为human还是robot

/*成五判断：在（x0,y0）点落下whom颜色的子，判断是否成五*/
bool win_or_not(int x0,int y0,bool whom);
/*用于记录是否禁手*/
bool ban_black;
/*用于记录思考时间*/
clock_t start,end;

//lianzhu2.c

/*定义棋形总数*/
#define TYPES_TOT 11
/*定义各种棋形*/
#define CHANG_LIAN 10
#define CHENG_5 9
#define HUO_4 8
#define CHONG_4 7
#define SI_4 6
#define _4_4 5
#define HUO_3 4
#define TIAO_HUO_3 3
#define MIAN_3 2
#define HUO_2 1
#define MIAN_2 0
/*定义棋盘上各点类型*/
#define WHITE 0b00
#define BLACK 0b01
#define EMPTY -1
#define BAN -2
#define EDGE -3

/*返回pattern对应的棋形：
 *第一维度输入pattern，第二维度输入禁手与否，第三维度输入己方颜色*/
int lianzhu_calc_map[300000][2][2];
/*用于pattern棋形的计算*/
void lianzhu_calc_find(int *pattern,int *x0,int d,int *n,bool *blank,bool consider_ban,int whom);
/*寻找特定pattern的棋形*/
int lianzhu_calc_gen(int *pattern,bool consider_ban,int whom);
/*生成pattern的棋形表*/
void lianzhu_calc_init();
/*返回（x0,y0）点dir方向的棋形*/
int lianzhu_calc(int x0,int y0,bool color,int dir,bool consider_ban);
/*用于存储特定点特定方向的棋形：前两维度输入点的横纵坐标，第三维度输入方向*/
int lianzhu[16][16][5];
/*判断（x0,y0）点是否为禁手*/
bool lianzhu_judge_ban(int x0,int y0);
/*刷新全棋盘的禁手点*/
void lianzhu_refresh_ban();
//void lianzhu_refresh_ban_at_dir_near(int x0,int y0,int dir_4);
//void lianzhu_refresh_ban_near(int x0,int y0);

//robot4.c

//int fg4_value[16][16][11];
//int fg4_x_self,fg4_y_self;
/*存储棋形打分表*/
int fg4_value_map[TYPES_TOT];
/*初始化棋形打分表*/
void fg4_value_map_init();
//int fg4_value_map(int pattern);
//int fg4_calc_value(int x0,int y0,bool color,int dir_4);
/*刷新（x0,y0）点附近点的估值*/
void fg4_refresh_value(int (*value)[16][11],int x0,int y0);
//void fg4();
//void nt4();
//void re4(bool is_black);

//robot6.c

/*标记己方颜色*/
bool fg6_self;
/*存储棋盘上每个点的估值：*/
/*第三维度0位存总评分，1-4位存黑棋评分，5-8存白棋评分，9存黑棋总分，10存白棋总分*/
int fg6_value[16][16][11];
/*记录上一时刻己方落子点，用于每次计算前刷新估值*/
int fg6_x_self,fg6_y_self;

/*定义搜索宽度及深度*/
#define MAX_WIDTH 12
#define BEGIN_STEP 8
#define BEGIN_WIDTH 12
#define BEGIN_DEPTH 11
#define IDEAL_WIDTH 12
#define IDEAL_DEPTH 11 //odd recommended
/*用于存储宽度*/
int WIDTH;
/*用于存储深度*/
int DEPTH;

/*linux系统下比较函数的实现*/
#define my_min(a, b)	\
({                      \
	typeof(a) _a = (a);	\
	typeof(b) _b = (b);	\
	_a <= _b ? _a : _b;	\
})
#define my_max(a, b)	\
({                      \
	typeof(a) _a = (a);	\
	typeof(b) _b = (b);	\
	_a >= _b ? _a : _b;	\
})

/*用于minmax底层棋盘总估值计算*/
int me_max;
/*用于minmax底层棋盘总估值计算*/
int thee_max;
/*用于minmax底层棋盘总估值计算*/
int deepest_value;

/*用于存储minmax搜索树*/
typedef struct tree{
    int value;
    int i;
    int j;
    bool searched;
    struct tree *son[MAX_WIDTH];
} tree;
/*minmax搜索树：用于避免minmax每层搜索中对最高估值点集的重复搜索*/
tree *fg6_tree;
/*根据对手落子点在上一轮搜索树中裁剪出子树*/
tree *tree_choose(tree *node,int i,int j);
//tree *tree_init(int value,int i,int j);
/*释放搜索树占用内存*/
void tree_free(tree **nodeptr);
/*计算落子node节点后的搜索得分*/
void fg6_calc_score(int alpha,int beta,tree *node,int depth,bool is_self);
/*寻找每层minmax估值最高的若干个点*/
void fg6_search_tops(bool banned,tree *node);
/*minmax搜索：每层根据估值最高点集进行下一层搜索，底层返回棋盘总估值*/
/*采用alpha-beta剪枝*/
int fg6_minmax(int alpha,int beta,tree *node,int depth,bool is_self);
/*minmax顶层搜索*/
int fg6_do_minmax();
/*根据第6代robot计算落子点*/
void fg6_calc(int depth);
/*第6代robot的计算函数*/
void fg6();
/*第6代robot的初始化函数*/
void nt6();
/*第6代robot的悔棋函数*/
void re6(bool is_black);
/*存储进攻状态还是防御状态*/
bool charge;

#endif