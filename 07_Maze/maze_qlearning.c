#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#define ROWS 10
#define COLS 10
#define ACTIONS 4  // 上右下左

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
    double qTable[ROWS][COLS][ACTIONS];  // 4个方向: 上右下左
    double epsilon;  // 探索率
    double alpha;    // 学习率
    double gamma;    // 折扣因子
} QLearner;

typedef struct {
    int x, y;
} Node;

bool isValid(int x, int y) {
    return x >= 0 && x < ROWS && y >= 0 && y < COLS && maze[x][y] == 0;
}

// 打印带路径的迷宫图
void printMazeWithPath(int pathLength, Node* pathNodes) {
    // 创建带路径的迷宫副本
    int mazeCopy[ROWS][COLS];
    memcpy(mazeCopy, maze, sizeof(maze));
    
    // 标记路径
    int i,j; 
    for (i = 0; i < pathLength; i++) {
        int x = pathNodes[i].x;
        int y = pathNodes[i].y;
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

QLearner* createQLearner() {
    QLearner* ql = malloc(sizeof(QLearner));
    memset(ql->qTable, 0, sizeof(ql->qTable));
    ql->epsilon = 0.1;
    ql->alpha = 0.1;
    ql->gamma = 0.9;
    return ql;
}

int getAction(QLearner* ql, int x, int y) {
    if ((double)rand() / RAND_MAX < ql->epsilon) {
        // 随机探索
        return rand() % ACTIONS;
    } else {
        // 利用已有知识
        double maxQ = ql->qTable[x][y][0];
        int bestAction = 0;
        
        int i;
        for (i = 1; i < ACTIONS; i++) {
            if (ql->qTable[x][y][i] > maxQ) {
                maxQ = ql->qTable[x][y][i];
                bestAction = i;
            }
        }
        
        return bestAction;
    }
}

int getNextState(int x, int y, int action) {
    int directions[ACTIONS][2] = {{-1,0}, {0,1}, {1,0}, {0,-1}};
    int nx = x + directions[action][0];
    int ny = y + directions[action][1];
    
    if (nx < 0 || nx >= ROWS || ny < 0 || ny >= COLS || maze[nx][ny] == 1) {
        return x * COLS + y;  // 无效移动，保持原位
    }
    
    return nx * COLS + ny;
}

void trainQLearner(QLearner* ql, int startX, int startY, int endX, int endY, int episodes) {
	int episode,i;
    for (episode = 0; episode < episodes; episode++) {
        int x = startX;
        int y = startY;
        bool done = false;
        
        while (!done) {
            int action = getAction(ql, x, y);
            int nextState = getNextState(x, y, action);
            int nx = nextState / COLS;
            int ny = nextState % COLS;
            
            // 计算奖励
            int reward = -1;  // 每步惩罚
            if (nx == endX && ny == endY) {
                reward = 100;  // 到达目标奖励
                done = true;
            }
            
            // Q学习更新
            double maxQNext = ql->qTable[nx][ny][0];
            for (i = 1; i < ACTIONS; i++) {
                if (ql->qTable[nx][ny][i] > maxQNext) {
                    maxQNext = ql->qTable[nx][ny][i];
                }
            }
            
            ql->qTable[x][y][action] += ql->alpha * (reward + ql->gamma * maxQNext - ql->qTable[x][y][action]);
            
            x = nx;
            y = ny;
        }
    }
}

bool solveWithQLearner(QLearner* ql, int startX, int startY, int endX, int endY) {
    int x = startX;
    int y = startY;
    Node pathNodes[ROWS * COLS];
    int pathLength = 0;
    bool visited[ROWS][COLS] = {0};
    
    pathNodes[pathLength].x = x;
    pathNodes[pathLength].y = y;
    pathLength++;
    visited[x][y] = true;
    
    while (x != endX || y != endY) {
        // 选择最优动作
        double maxQ = ql->qTable[x][y][0];
        int bestAction = 0;
        
        int i;
        for (i = 1; i < ACTIONS; i++) {
            if (ql->qTable[x][y][i] > maxQ) {
                maxQ = ql->qTable[x][y][i];
                bestAction = i;
            }
        }
        
        // 执行动作
        int nextState = getNextState(x, y, bestAction);
        int nx = nextState / COLS;
        int ny = nextState % COLS;
        
        if (visited[nx][ny]) {
            printf("Q学习陷入循环，未找到有效路径！\n");
            return false;
        }
        
        x = nx;
        y = ny;
        visited[x][y] = true;
        
        // 记录路径
        pathNodes[pathLength].x = x;
        pathNodes[pathLength].y = y;
        pathLength++;
    }
    
    // 输出路径
    printMazeWithPath(pathLength, pathNodes);
    printf("找到路径！步数: %d\n", pathLength);
    return true;
}

int main() {
    srand(time(NULL));
    QLearner* ql = createQLearner();
    
    // 训练Q学习
    printf("开始训练Q学习算法...\n");
    trainQLearner(ql, 0, 0, 9, 9, 1000);
    
    // 使用Q学习求解迷宫
    printf("开始求解迷宫...\n");
    if (!solveWithQLearner(ql, 0, 0, 9, 9)) {
        printf("未能找到有效路径。\n");
    }
    
    free(ql);
    return 0;
}
