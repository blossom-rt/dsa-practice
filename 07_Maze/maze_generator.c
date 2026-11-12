#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define ROWS 10
#define COLS 10

// 迷宫地图（0=通路，1=墙，2=路径）
int maze[ROWS][COLS];

// 初始化迷宫为全通路
void initMaze() {
	int i,j; 
    for (i = 0; i < ROWS; i++) {
        for (j = 0; j < COLS; j++) {
            maze[i][j] = 0;
        }
    }
}

// 打印迷宫
void printMaze() {
    printf("\n迷宫布局:\n");
    int i,j;
    for (i = 0; i < ROWS; i++) {
        for (j = 0; j < COLS; j++) {
            if (maze[i][j] == 0) printf("0 ");   // 通路
            else if (maze[i][j] == 1) printf("# ");  // 墙
            else if (maze[i][j] == 2) printf("* ");  // 路径
        }
        printf("\n");
    }
}

// 随机生成简单迷宫（低墙密度）
void generateSimpleMaze() {
    initMaze();
    
    // 设置起点和终点为通路
    maze[0][0] = 0;
    maze[ROWS-1][COLS-1] = 0;
    
    // 随机放置墙，密度约为20%
    int i,j;
    for (i = 0; i < ROWS; i++) {
        for (j = 0; j < COLS; j++) {
            // 避开起点和终点
            if ((i == 0 && j == 0) || (i == ROWS-1 && j == COLS-1)) continue;
            
            // 20%概率放置墙
            if (rand() % 5 == 0) {
                maze[i][j] = 1;
            }
        }
    }
    
    // 确保起点和终点周围没有墙
    if (ROWS > 1) maze[1][0] = 0;
    if (COLS > 1) maze[0][1] = 0;
    if (ROWS > 1) maze[ROWS-2][COLS-1] = 0;
    if (COLS > 1) maze[ROWS-1][COLS-2] = 0;
}

// 随机生成复杂迷宫（高墙密度，曲折路径）
void generateComplexMaze() {
    initMaze();
    
    // 设置起点和终点为通路
    maze[0][0] = 0;
    maze[ROWS-1][COLS-1] = 0;
    
    // 随机放置墙，密度约为40%
    int i,j;
    for (i = 0; i < ROWS; i++) {
        for (j = 0; j < COLS; j++) {
            // 避开起点和终点
            if ((i == 0 && j == 0) || (i == ROWS-1 && j == COLS-1)) continue;
            
            // 40%概率放置墙
            if (rand() % 10 < 4) {
                maze[i][j] = 1;
            }
        }
    }
    
    // 确保起点和终点周围没有墙
    if (ROWS > 1) maze[1][0] = 0;
    if (COLS > 1) maze[0][1] = 0;
    if (ROWS > 1) maze[ROWS-2][COLS-1] = 0;
    if (COLS > 1) maze[ROWS-1][COLS-2] = 0;
    
    // 增加一些长墙以增加复杂度
    for (i = 0; i < 5; i++) {
        int wallType = rand() % 2;  // 0=水平墙，1=垂直墙
        int pos = rand() % ROWS;
        int start, end;
        
        if (wallType == 0) {  // 水平墙
            start = rand() % (COLS - 3);
            end = start + 2 + rand() % (COLS - start - 2);
            for (j = start; j <= end; j++) {
                if (pos == 0 && j == 0) continue;  // 避开起点
                if (pos == ROWS-1 && j == COLS-1) continue;  // 避开终点
                maze[pos][j] = 1;
            }
        } else {  // 垂直墙
            start = rand() % (ROWS - 3);
            end = start + 2 + rand() % (ROWS - start - 2);
            for (j = start; j <= end; j++) {
                if (j == 0 && pos == 0) continue;  // 避开起点
                if (j == ROWS-1 && pos == COLS-1) continue;  // 避开终点
                maze[j][pos] = 1;
            }
        }
    }
}

// 随机生成不可解迷宫（起点和终点被墙完全隔开）
void generateUnsolvableMaze() {
    initMaze();
    
    // 设置起点和终点为通路
    maze[0][0] = 0;
    maze[ROWS-1][COLS-1] = 0;
    
    // 放置一堵垂直或水平的墙将迷宫分成两半
    if (rand() % 2 == 0) {  // 垂直墙
        int wallPos = 2 + rand() % (COLS - 4);
        int i;
        for (i = 0; i < ROWS; i++) {
            maze[i][wallPos] = 1;
        }
    } else {  // 水平墙
        int wallPos = 2 + rand() % (ROWS - 4);
        int j;
        for (j = 0; j < COLS; j++) {
            maze[wallPos][j] = 1;
        }
    }
    
    // 随机放置额外的墙，密度约为20%
    int i,j;
    for (i = 0; i < ROWS; i++) {
        for (j = 0; j < COLS; j++) {
            // 避开起点和终点
            if ((i == 0 && j == 0) || (i == ROWS-1 && j == COLS-1)) continue;
            
            // 20%概率放置墙
            if (rand() % 5 == 0) {
                maze[i][j] = 1;
            }
        }
    }
}

int main() {
    srand(time(NULL));  // 初始化随机数种子

    // 生成并测试简单迷宫
    printf("简单迷宫\n");
    generateSimpleMaze();
    printMaze();
    
    // 生成并测试复杂迷宫
    printf("复杂迷宫\n");
    generateComplexMaze();
    printMaze();
    
    // 生成并测试不可解迷宫
    printf("不可解迷宫\n");
    generateUnsolvableMaze();
    printMaze();
    
    return 0;
}
