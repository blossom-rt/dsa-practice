#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ROWS 10
#define COLS 10

// 迷宫单元格类型 - 增加结构体标签 Node
typedef struct Node {
    int x, y;         // 坐标
    struct Node* prev; // 前驱节点
} Node;

// 堆栈结构
typedef struct {
    Node* data[1000];
    int top;
} Stack;

// 初始化堆栈
void initStack(Stack* s) {
    s->top = -1;
}

// 判断堆栈是否为空
int isEmpty(Stack* s) {
    return s->top == -1;
}

// 入栈
void push(Stack* s, Node* node) {
    s->data[++(s->top)] = node;
}

// 出栈
Node* pop(Stack* s) {
    return s->data[(s->top)--];
}

// 迷宫定义
int maze[ROWS][COLS] = {
    {0, 1, 0, 0, 0, 0, 1, 0, 0, 0},
    {0, 1, 0, 1, 1, 0, 1, 0, 1, 0},
    {0, 0, 0, 1, 0, 0, 0, 0, 0, 0},
    {0, 1, 1, 1, 0, 1, 1, 1, 1, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
    {1, 1, 1, 1, 1, 1, 1, 0, 1, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
    {0, 1, 1, 1, 1, 1, 1, 1, 1, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 1, 1, 1, 1, 0, 1, 1, 1, 0}
};

// 检查坐标是否合法且可通行
int isValid(int x, int y) {
    return x >= 0 && x < ROWS && y >= 0 && y < COLS && maze[x][y] == 0;
}

// 打印迷宫
void printMaze() {
    printf("迷宫布局:\n");
    int i,j; 
    for (i = 0; i < ROWS; i++) {
        for (j = 0; j < COLS; j++) {
            printf("%c ", maze[i][j] ? '#' : '0');
        }
        printf("\n");
    }
}

// 打印带路径的迷宫
void printMazeWithPath(Node* path) {
    int pathMaze[ROWS][COLS];
    memcpy(pathMaze, maze, sizeof(maze));
    
    // 标记路径
    Node* current = path;
    while (current != NULL) {
        pathMaze[current->x][current->y] = 2; // 2表示路径
        current = current->prev;
    }
    
    printf("\n迷宫路径可视化:\n");
    int i,j;
    for (i = 0; i < ROWS; i++) {
        for (j = 0; j < COLS; j++) {
            if (pathMaze[i][j] == 1) printf("# ");  // 墙
            else if (pathMaze[i][j] == 2) printf("* "); // 路径
            else printf("0 ");  // 可通行区域
        }
        printf("\n");
    }
}

// 递归DFS寻找路径
Node* dfsRecursive(int x, int y, int visited[ROWS][COLS]) {
    // 检查是否到达终点(右下角)
    if (x == ROWS - 1 && y == COLS - 1) {
        Node* node = (Node*)malloc(sizeof(Node));
        node->x = x;
        node->y = y;
        node->prev = NULL;
        return node;
    }
    
    // 标记当前位置为已访问
    visited[x][y] = 1;
    
    // 定义四个方向：上、右、下、左
    int dx[] = {-1, 0, 1, 0};
    int dy[] = {0, 1, 0, -1};
    
    // 尝试四个方向
    int i;
    for (i = 0; i < 4; i++) {
        int nx = x + dx[i];
        int ny = y + dy[i];
        
        if (isValid(nx, ny) && !visited[nx][ny]) {
            Node* path = dfsRecursive(nx, ny, visited);
            if (path != NULL) {
                Node* node = (Node*)malloc(sizeof(Node));
                node->x = x;
                node->y = y;
                node->prev = path;
                return node;
            }
        }
    }
    
    return NULL; // 没有找到路径
}

// 非递归DFS寻找路径
Node* dfsNonRecursive(int startX, int startY) {
    Stack stack;
    initStack(&stack);
    
    int visited[ROWS][COLS] = {0};
    
    Node* start = (Node*)malloc(sizeof(Node));
    start->x = startX;
    start->y = startY;
    start->prev = NULL;
    
    push(&stack, start);
    visited[startX][startY] = 1;
    
    while (!isEmpty(&stack)) {
        Node* current = pop(&stack);
        
        // 检查是否到达终点
        if (current->x == ROWS - 1 && current->y == COLS - 1) {
            return current;
        }
        
        // 定义四个方向：上、右、下、左
        int dx[] = {-1, 0, 1, 0};
        int dy[] = {0, 1, 0, -1};
        
        // 尝试四个方向
        int i;
        for (i = 0; i < 4; i++) {
            int nx = current->x + dx[i];
            int ny = current->y + dy[i];
            
            if (isValid(nx, ny) && !visited[nx][ny]) {
                Node* next = (Node*)malloc(sizeof(Node));
                next->x = nx;
                next->y = ny;
                next->prev = current;
                
                push(&stack, next);
                visited[nx][ny] = 1;
            }
        }
    }
    
    return NULL; // 没有找到路径
}

// 释放路径内存
void freePath(Node* path) {
    while (path != NULL) {
        Node* temp = path;
        path = path->prev;
        free(temp);
    }
}

int main() {
    printMaze();
    
    // 使用递归DFS寻找路径
    printf("\n使用递归DFS寻找路径:\n");
    int visited[ROWS][COLS] = {0};
    Node* pathRecursive = dfsRecursive(0, 0, visited);
    
    if (pathRecursive != NULL) {
        printMazeWithPath(pathRecursive);
        freePath(pathRecursive);
    } else {
        printf("没有找到路径!\n");
    }
    
    // 使用非递归DFS寻找路径
    printf("\n使用非递归DFS寻找路径:\n");
    Node* pathNonRecursive = dfsNonRecursive(0, 0);
    
    if (pathNonRecursive != NULL) {
        printMazeWithPath(pathNonRecursive);
        freePath(pathNonRecursive);
    } else {
        printf("没有找到路径!\n");
    }
    
    return 0;
}
