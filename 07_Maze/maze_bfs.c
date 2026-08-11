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

// 队列结构用于BFS
typedef struct {
    Point* data;
    int front, rear;
    int capacity;
} Queue;

Queue* createQueue(int capacity) {
    Queue* q = (Queue*)malloc(sizeof(Queue));
    q->data = (Point*)malloc(capacity * sizeof(Point));
    q->front = q->rear = 0;
    q->capacity = capacity;
    return q;
}

void destroyQueue(Queue* q) {
    free(q->data);
    free(q);
}

bool isEmpty(Queue* q) {
    return q->front == q->rear;
}

void enqueue(Queue* q, Point p) {
    q->data[(q->rear)++] = p;
}

Point dequeue(Queue* q) {
    return q->data[(q->front)++];
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

bool bfs(int startX, int startY, int endX, int endY) {
    if (!isValid(startX, startY) || !isValid(endX, endY)) {
        printf("起点或终点无效！\n");
        return false;
    }
    
    bool visited[ROWS][COLS] = {false};
    Point parent[ROWS][COLS];  // 记录路径
    Queue* queue = createQueue(ROWS * COLS);
    Point path[ROWS * COLS];
    int pathLength = 0;
    
    // 四个方向：上、下、左、右
    int directions[4][2] = {{-1,0}, {1,0}, {0,-1}, {0,1}};
    int i;
    
    Point start = {startX, startY};
    enqueue(queue, start);
    visited[startX][startY] = true;
    parent[startX][startY] = (Point){-1, -1};  // 起点的父节点标记为(-1,-1)
    
    while (!isEmpty(queue)) {
        Point current = dequeue(queue);
        
        // 到达终点
        if (current.x == endX && current.y == endY) {
            // 构建路径
            Point p = current;
            while (p.x != -1 && p.y != -1) {
                path[pathLength++] = p;
                p = parent[p.x][p.y];
            }
            
            // 反转路径
            for (i = 0; i < pathLength / 2; i++) {
                Point temp = path[i];
                path[i] = path[pathLength - i - 1];
                path[pathLength - i - 1] = temp;
            }
            
            printf("BFS找到路径！路径长度: %d\n", pathLength);
            printf("路径坐标:\n");
            for (i = 0; i < pathLength; i++) {
                printf("(%d, %d)\n", path[i].x, path[i].y);
            }
            
            printMazeWithPath(path, pathLength);
            
            destroyQueue(queue);
            return true;
        }
        
        // 扩展当前节点
        for (i = 0; i < 4; i++) {
            int nx = current.x + directions[i][0];
            int ny = current.y + directions[i][1];
            
            if (isValid(nx, ny) && !visited[nx][ny]) {
                enqueue(queue, (Point){nx, ny});
                visited[nx][ny] = true;
                parent[nx][ny] = current;
            }
        }
    }
    
    printf("BFS未找到路径！\n");
    destroyQueue(queue);
    return false;
}

int main() {
    int startX = 0, startY = 0;
    int endX = ROWS - 1, endY = COLS - 1;
    
    printf("广度优先搜索(BFS)求解老鼠走迷宫问题\n");
    printf("起点: (%d, %d), 终点: (%d, %d)\n\n", startX, startY, endX, endY);
    
    bfs(startX, startY, endX, endY);
    
    return 0;
}
