/**********************************************************************************************
Copyright (c) 2021 LazyBird
File name:      wuziqi.h
Description:    ����������ͷ�ļ���������robot4.c robot6.c lianzhu2.c������к�����wuziqi.c��Ĳ��ֺ���
Author:         Eric Jin
Date:           2021.12.31
**********************************************************************************************/
#ifndef __WUZIQI_H__
#define __WUZIQI_H__

/*���ʹ���°����̨���뱣���ú궨�壬��֮��ע��*/
#define NEW_CMD

#pragma comment(linker, "/STACK:102400000,102400000")
#include "opt.h"//gcc�Ż�ͷ�ļ�
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>

//wuziqi.c

/*���ݲ���ϵͳѡ����Ӧ��sleep��clear����*/
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#include <conio.h>
#include <windows.h>
#define sleep(a) Sleep(1000*(a))
#define clear() system("cls")
#elif __linux__
#include <unistd.h>
#define clear() system("clear")
#endif

/*���ӻ�����*/
struct robot{
    void (*nt)();               //initialize�����ڳ�ʼ��������
    void (*fg)();               //figure�����ڼ������ӵ�
    void (*re)(bool is_black);  //retract�����ڻ���
} black_robot,white_robot;

/*ʹ��4+15+4=23�Ĵ洢��ʽ��Ԥ��4���㣬�����жϱ߽�*/
int board_expanded[23][23];
/*���̣�1~15Ϊ������Ч��Χ��-3~0��16~19Ϊ�߽緶Χ*/
int (*board)[23];

typedef struct point{
    int x,y;
}point;
/*ʱ���ߣ���¼ÿ�����ӵĺ������꣬���ڻ���*/
point timeline[15*15+2];

int step;           //��¼��ǰ����
int x,y;            //��ǰ���ӵ�
int last_x,last_y;  //��һʱ�̶������ӵ�
int err;            //������Ϣ������Ϊ0
bool is_robot[2];   //��¼�ڰ�˫���ֱ�Ϊhuman����robot

/*�����жϣ��ڣ�x0,y0��������whom��ɫ���ӣ��ж��Ƿ����*/
bool win_or_not(int x0,int y0,bool whom);
/*���ڼ�¼�Ƿ����*/
bool ban_black;
/*���ڼ�¼˼��ʱ��*/
clock_t start,end;

//lianzhu2.c

/*������������*/
#define TYPES_TOT 11
/*�����������*/
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
/*���������ϸ�������*/
#define WHITE 0b00
#define BLACK 0b01
#define EMPTY -1
#define BAN -2
#define EDGE -3

/*����pattern��Ӧ�����Σ�
 *��һά������pattern���ڶ�ά�����������񣬵���ά�����뼺����ɫ*/
int lianzhu_calc_map[300000][2][2];
/*����pattern���εļ���*/
void lianzhu_calc_find(int *pattern,int *x0,int d,int *n,bool *blank,bool consider_ban,int whom);
/*Ѱ���ض�pattern������*/
int lianzhu_calc_gen(int *pattern,bool consider_ban,int whom);
/*����pattern�����α�*/
void lianzhu_calc_init();
/*���أ�x0,y0����dir���������*/
int lianzhu_calc(int x0,int y0,bool color,int dir,bool consider_ban);
/*���ڴ洢�ض����ض���������Σ�ǰ��ά�������ĺ������꣬����ά�����뷽��*/
int lianzhu[16][16][5];
/*�жϣ�x0,y0�����Ƿ�Ϊ����*/
bool lianzhu_judge_ban(int x0,int y0);
/*ˢ��ȫ���̵Ľ��ֵ�*/
void lianzhu_refresh_ban();
//void lianzhu_refresh_ban_at_dir_near(int x0,int y0,int dir_4);
//void lianzhu_refresh_ban_near(int x0,int y0);

//robot4.c

//int fg4_value[16][16][11];
//int fg4_x_self,fg4_y_self;
/*�洢���δ�ֱ�*/
int fg4_value_map[TYPES_TOT];
/*��ʼ�����δ�ֱ�*/
void fg4_value_map_init();
//int fg4_value_map(int pattern);
//int fg4_calc_value(int x0,int y0,bool color,int dir_4);
/*ˢ�£�x0,y0���㸽����Ĺ�ֵ*/
void fg4_refresh_value(int (*value)[16][11],int x0,int y0);
//void fg4();
//void nt4();
//void re4(bool is_black);

//robot6.c

/*��Ǽ�����ɫ*/
bool fg6_self;
/*�洢������ÿ����Ĺ�ֵ��*/
/*����ά��0λ�������֣�1-4λ��������֣�5-8��������֣�9������ܷ֣�10������ܷ�*/
int fg6_value[16][16][11];
/*��¼��һʱ�̼������ӵ㣬����ÿ�μ���ǰˢ�¹�ֵ*/
int fg6_x_self,fg6_y_self;

/*����������ȼ����*/
#define MAX_WIDTH 12
#define BEGIN_STEP 8
#define BEGIN_WIDTH 12
#define BEGIN_DEPTH 11
#define IDEAL_WIDTH 12
#define IDEAL_DEPTH 11 //odd recommended
/*���ڴ洢���*/
int WIDTH;
/*���ڴ洢���*/
int DEPTH;

/*linuxϵͳ�±ȽϺ�����ʵ��*/
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

/*����minmax�ײ������ܹ�ֵ����*/
int me_max;
/*����minmax�ײ������ܹ�ֵ����*/
int thee_max;
/*����minmax�ײ������ܹ�ֵ����*/
int deepest_value;

/*���ڴ洢minmax������*/
typedef struct tree{
    int value;
    int i;
    int j;
    bool searched;
    struct tree *son[MAX_WIDTH];
} tree;
/*minmax�����������ڱ���minmaxÿ�������ж���߹�ֵ�㼯���ظ�����*/
tree *fg6_tree;
/*���ݶ������ӵ�����һ���������вü�������*/
tree *tree_choose(tree *node,int i,int j);
//tree *tree_init(int value,int i,int j);
/*�ͷ�������ռ���ڴ�*/
void tree_free(tree **nodeptr);
/*��������node�ڵ��������÷�*/
void fg6_calc_score(int alpha,int beta,tree *node,int depth,bool is_self);
/*Ѱ��ÿ��minmax��ֵ��ߵ����ɸ���*/
void fg6_search_tops(bool banned,tree *node);
/*minmax������ÿ����ݹ�ֵ��ߵ㼯������һ���������ײ㷵�������ܹ�ֵ*/
/*����alpha-beta��֦*/
int fg6_minmax(int alpha,int beta,tree *node,int depth,bool is_self);
/*minmax��������*/
int fg6_do_minmax();
/*���ݵ�6��robot�������ӵ�*/
void fg6_calc(int depth);
/*��6��robot�ļ��㺯��*/
void fg6();
/*��6��robot�ĳ�ʼ������*/
void nt6();
/*��6��robot�Ļ��庯��*/
void re6(bool is_black);
/*�洢����״̬���Ƿ���״̬*/
bool charge;

#endif