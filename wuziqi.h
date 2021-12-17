#ifndef __WUZIQI_H__
#define __WUZIQI_H__
#pragma comment(linker, "/STACK:102400000,102400000")
#include "opt.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>

//wuziqi.c

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#include <conio.h>
#include <windows.h>
#define sleep(a) Sleep(1000*(a))
#define clear() system("cls")
#elif __linux__
#include <unistd.h>
#define clear() system("clear")
#endif

struct robot{
    void (*nt)();
    void (*fg)();
    void (*re)(bool is_black);
} black_robot,white_robot;

int board_expanded[23][23];
int (*board)[23];
typedef struct point{
    int x,y;
}point;
point timeline[15*15+2];

int step;
int x,y;
int last_x,last_y;
int err;
bool is_robot[2];

bool win_or_not(int x0,int y0,bool whom);

bool ban_black;

clock_t start,end;


//lianzhu2.c

#define TYPES_TOT 11
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

#define WHITE 0b00
#define BLACK 0b01
#define EMPTY -1
#define BAN -2
#define EDGE -3

int lianzhu_calc_map[300000][2][2];
void lianzhu_calc_find(int *pattern,int *x0,int d,int *n,bool *blank,bool consider_ban,int whom);
int lianzhu_calc_gen(int *pattern,bool consider_ban,int whom);
void lianzhu_calc_init();
int lianzhu_calc(int x0,int y0,bool color,int dir,bool consider_ban);
int lianzhu[16][16][5];
bool lianzhu_judge_ban(int x0,int y0);
void lianzhu_refresh_ban();
void lianzhu_refresh_ban_at_dir_near(int x0,int y0,int dir_4);
void lianzhu_refresh_ban_near(int x0,int y0);


//robot4.c

//第三维度0位存总评分，1-4位存黑棋评分，5-8存白棋评分，9存黑棋总分，10存白棋总分
int fg4_value[16][16][11];
int fg4_x_self,fg4_y_self;
int fg4_value_map[TYPES_TOT];

void fg4_value_map_init();
//int fg4_value_map(int pattern);
int fg4_calc_value(int x0,int y0,bool color,int dir_4);
void fg4_refresh_value(int (*value)[16][11],int x0,int y0);
void fg4();
void nt4();
void re4(bool is_black);


//robot6.c

bool fg6_self;
int fg6_value[16][16][11];
int fg6_x_self,fg6_y_self;

#define MAX_WIDTH 13
#define BEGIN_STEP 8
#define BEGIN_WIDTH 13
#define BEGIN_DEPTH 11//7
#define IDEAL_WIDTH 13//6
#define IDEAL_DEPTH 11 //odd recommended

int WIDTH;
int DEPTH;

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

int me_max;
int thee_max;

typedef struct tree{
    int value;
    int i;
    int j;
    bool searched;
    struct tree* son[MAX_WIDTH];
} tree;
tree *fg6_tree;

tree *tree_choose(tree *node,int i,int j);
tree *tree_init(int value,int i,int j);
void tree_free(tree **nodeptr);
void fg6_calc_score(int alpha,int beta,tree *node,int depth,bool is_self);
void fg6_search_tops(bool banned,tree *node);
int fg6_minmax(int alpha,int beta,tree *node,int depth,bool is_self);
void fg6_calc(int depth);
void fg6();
void nt6();
void re6(bool is_black);

#endif