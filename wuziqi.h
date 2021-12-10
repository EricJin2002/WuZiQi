#pragma once
#include "opt.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#include <conio.h>
#include <windows.h>
#define sleep(a) Sleep(1000*(a))
#define clear() system("cls")
#elif __linux__
#include <unistd.h>
#define clear() system("clear")
#endif

#define black 1
#define white 0

struct robot{
    void (*nt)();
    void (*fg)();
    void (*re)(bool is_black);
} black_robot,white_robot;

int board[16][16];
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