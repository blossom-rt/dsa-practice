#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define ROWS 10
#define COLS 10

// 定义Node类型
typedef struct Node {
    int x, y;
} Node;

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

typedef struct MemoryNode {
    int x, y;
    int direction;  // 0=上，1=右，2=下，3=左
    struct MemoryNode* next;
} MemoryNode;

MemoryNode* memoryHead = NULL;
Node* pathNodes[ROWS * COLS];  // 存储路径节点
int pathLength = 0;

bool isValid(int x, int y) {
    return x >= 0 && x < ROWS && y >= 0 && y < COLS && maze[x][y] == 0;
}

// 创建节点函数
Node* createNode(int x, int y) {
    Node* node = malloc(sizeof(Node));
    node->x = x;
    node->y = y;
    return node;
}

// 打印带路径的迷宫图
void printMazeWithPath() {
    // 创建带路径的迷宫副本
    int mazeCopy[ROWS][COLS];
    memcpy(mazeCopy, maze, sizeof(maze));
    
    // 标记路径
    int i,j; 
    for (i = 0; i < pathLength; i++) {
        int x = pathNodes[i]->x;
        int y = pathNodes[i]->y;
        mazeCopy[x][y] = 2;  // 2表示路径
    }
    
    // 输出迷宫图
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

bool memoryDFS(int x, int y, int endX, int endY, bool** visited) {
    if (x == endX && y == endY) {
        pathNodes[pathLength++] = createNode(x, y);
        return true;
    }
    
    if (x < 0 || x >= ROWS || y < 0 || y >= COLS || maze[x][y] == 1 || visited[x][y]) return false;
    
    visited[x][y] = true;
    pathNodes[pathLength++] = createNode(x, y);
    
    // 尝试四个方向（上、右、下、左）
    int directions[4][2] = {{-1,0}, {0,1}, {1,0}, {0,-1}};
    int i;
    for (i = 0; i < 4; i++) {
        int nx = x + directions[i][0];
        int ny = y + directions[i][1];
        
        if (memoryDFS(nx, ny, endX, endY, visited)) {
            // 记录正确方向
            MemoryNode* node = malloc(sizeof(MemoryNode));
            node->x = x;
            node->y = y;
            node->direction = i;
            node->next = memoryHead;
            memoryHead = node;
            return true;
        }
    }
    
    // 回溯
    pathLength--;
    return false;
}

void freeMemory() {
    while (memoryHead != NULL) {
        MemoryNode* temp = memoryHead;
        memoryHead = memoryHead->next;
        free(temp);
    }
    int i;
    for (i = 0; i < pathLength; i++) {
        free(pathNodes[i]);
    }
}

int main() {
    int startX = 0, startY = 0;
    int endX = ROWS - 1, endY = COLS - 1;
    
    printf("记忆DFS算法求解老鼠走迷宫问题\n");
    printf("起点: (%d, %d), 终点: (%d, %d)\n\n", startX, startY, endX, endY);
    
    bool** visited = malloc(ROWS * sizeof(bool*));
    int i;
    for (i = 0; i < ROWS; i++) {
        visited[i] = malloc(COLS * sizeof(bool));
        memset(visited[i], false, COLS * sizeof(bool));
    }
    
    if (memoryDFS(startX, startY, endX, endY, visited)) {
        printf("记忆DFS找到路径！路径长度: %d\n", pathLength);
        printf("路径坐标:\n");
        for (i = 0; i < pathLength; i++) {
            printf("(%d, %d)\n", pathNodes[i]->x, pathNodes[i]->y);
        }
        
        // 输出迷宫图
        printMazeWithPath();
    } else {
        printf("记忆DFS未找到路径！\n");
    }
    
    for (i = 0; i < ROWS; i++) {
        free(visited[i]);
    }
    free(visited);
    freeMemory();
    
    return 0;
}    
