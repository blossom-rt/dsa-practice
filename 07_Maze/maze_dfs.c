#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define ROWS 10
#define COLS 10

// 迷宫定义（0=通路，1=墙，2=路径）
int maze[ROWS][COLS] = {
    {0,1,0,0,0,0,0,0,0,0},
    {0,1,0,1,1,1,1,1,1,0},
    {0,1,0,0,0,0,0,0,0,0},
    {0,1,1,1,1,1,1,1,1,0},
    {0,0,0,0,0,0,0,0,1,0},
    {0,1,1,1,1,1,1,0,1,0},
    {0,1,0,0,0,0,0,0,1,0},
    {0,1,0,1,1,1,1,0,1,0},
    {0,0,0,0,0,0,0,0,1,0},
    {0,0,0,0,0,1,0,0,0,0}
};

typedef struct {
    int x, y;
} Point;

// 栈结构用于DFS
typedef struct {
    Point* data;
    int top;
    int capacity;
} Stack;

Stack* createStack(int capacity) {
    Stack* s = (Stack*)malloc(sizeof(Stack));
    s->data = (Point*)malloc(capacity * sizeof(Point));
    s->top = -1;
    s->capacity = capacity;
    return s;
}

void destroyStack(Stack* s) {
    free(s->data);
    free(s);
}

bool isEmpty(Stack* s) {
    return s->top == -1;
}

void push(Stack* s, Point p) {
    s->data[++(s->top)] = p;
}

Point pop(Stack* s) {
    return s->data[(s->top)--];
}

Point peek(Stack* s) {
    return s->data[s->top];
}

bool isValid(int x, int y) {
    return x >= 0 && x < ROWS && y >= 0 && y < COLS && maze[x][y] == 0;
}

// 打印带路径的迷宫图
void printMazeWithPath(Point* path, int pathLength) {
    int mazeCopy[ROWS][COLS],i,j;
    memcpy(mazeCopy, maze, sizeof(maze));
    
    for (i = 0; i < pathLength; i++) {
        mazeCopy[path[i].x][path[i].y] = 2;
    }
    
    printf("\n迷宫路径可视化:\n");
    for (i = 0; i < ROWS; i++) {
        for (j = 0; j < COLS; j++) {
            if (mazeCopy[i][j] == 0) printf("0 ");   // 通路
            else if (mazeCopy[i][j] == 1) printf("# ");  // 墙
            else if (mazeCopy[i][j] == 2) printf("* ");  // 路径
        }
        printf("\n");
    }
}

bool dfs(int startX, int startY, int endX, int endY) {
    if (!isValid(startX, startY) || !isValid(endX, endY)) {
        printf("起点或终点无效！\n");
        return false;
    }
    
    bool visited[ROWS][COLS] = {false};
    Stack* stack = createStack(ROWS * COLS);
    Point path[ROWS * COLS];
    int pathLength = 0;
    
    // 四个方向：上、下、左、右
    int directions[4][2] = {{-1,0}, {1,0}, {0,-1}, {0,1}};
    int i;

    Point start = {startX, startY};
    push(stack, start);
    visited[startX][startY] = true;
    
    while (!isEmpty(stack)) {
        Point current = peek(stack);
        
        // 到达终点
        if (current.x == endX && current.y == endY) {
            // 构建路径
            pathLength = stack->top + 1;
            for (i = 0; i <= stack->top; i++) {
                path[i] = stack->data[i];
            }
            
            printf("DFS找到路径！路径长度: %d\n", pathLength);
            printf("路径坐标:\n");
            for (i = 0; i < pathLength; i++) {
                printf("(%d, %d)\n", path[i].x, path[i].y);
            }
            
            printMazeWithPath(path, pathLength);
            
            destroyStack(stack);
            return true;
        }
        
        bool found = false;
        for (i = 0; i < 4; i++) {
            int nx = current.x + directions[i][0];
            int ny = current.y + directions[i][1];
            
            if (isValid(nx, ny) && !visited[nx][ny]) {
                push(stack, (Point){nx, ny});
                visited[nx][ny] = true;
                found = true;
                break;
            }
        }
        
        // 如果没有找到可走的方向，回溯
        if (!found) {
            pop(stack);
        }
    }
    
    printf("DFS未找到路径！\n");
    destroyStack(stack);
    return false;
}

int main() {
    int startX = 0, startY = 0;
    int endX = ROWS - 1, endY = COLS - 1;
    
    printf("深度优先搜索(DFS)求解老鼠走迷宫问题\n");
    printf("起点: (%d, %d), 终点: (%d, %d)\n\n", startX, startY, endX, endY);
    
    dfs(startX, startY, endX, endY);
    
    return 0;
}
