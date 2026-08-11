#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <stdbool.h>  // 添加布尔类型支持

#define ROWS 10
#define COLS 10
#define ACTIONS 4  // 上右下左
#define EPISODES 2000  // 训练次数
#define MAX_EPISODE_STEPS 500  // 最大步数

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
    double qTable[ROWS][COLS][ACTIONS];  // Q值表
    double alpha;     // 学习率
    double gamma;     // 折扣因子
    double epsilon;   // 探索率
} QLearner;

// 方向：上、右、下、左
int directions[ACTIONS][2] = {{-1,0}, {0,1}, {1,0}, {0,-1}};

bool isValid(int x, int y) {
    return x >= 0 && x < ROWS && y >= 0 && y < COLS && maze[x][y] == 0;
}

// 欧几里得距离启发函数
double heuristic(int x1, int y1, int x2, int y2) {
    return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
}

// 初始化QLearner
QLearner* createQLearner(double alpha, double gamma, double epsilon) {
    QLearner* ql = (QLearner*)malloc(sizeof(QLearner));
    memset(ql->qTable, 0, sizeof(ql->qTable));
    ql->alpha = alpha;
    ql->gamma = gamma;
    ql->epsilon = epsilon;
    return ql;
}

// 用启发式函数初始化Q表
void initializeQTableWithHeuristic(QLearner* ql, int goalX, int goalY) {
	int x,y,a;
    for (x = 0; x < ROWS; x++) {
        for (y = 0; y < COLS; y++) {
            if (maze[x][y] == 1) continue;  // 跳过墙
            
            for (a = 0; a < ACTIONS; a++) {
                int nx = x + directions[a][0];
                int ny = y + directions[a][1];
                
                if (isValid(nx, ny)) {
                    // 启发式值：当前位置到目标的距离
                    double h_current = heuristic(x, y, goalX, goalY);
                    // 移动后位置到目标的距离
                    double h_next = heuristic(nx, ny, goalX, goalY);
                    
                    // 如果移动后更接近目标，赋予更高Q值
                    if (h_next < h_current) {
                        ql->qTable[x][y][a] = 10.0 * (h_current - h_next);
                    }
                }
            }
        }
    }
}

// epsilon-贪婪策略选择动作
int chooseAction(QLearner* ql, int x, int y, int episode) {
    // 动态调整探索率：随着训练进行，减少随机探索
    double epsilon = ql->epsilon * (1 - (double)episode / EPISODES);
    
    if ((double)rand() / RAND_MAX < epsilon) {
        // 探索：随机选择动作
        return rand() % ACTIONS;
    } else {
        // 利用：选择Q值最大的动作
        int bestAction = 0;
        double maxQ = ql->qTable[x][y][0];
        
        int a;
        for (a = 1; a < ACTIONS; a++) {
            if (ql->qTable[x][y][a] > maxQ) {
                maxQ = ql->qTable[x][y][a];
                bestAction = a;
            }
        }
        
        return bestAction;
    }
}

// 更新Q值
void updateQValue(QLearner* ql, int x, int y, int action, int reward, int nextX, int nextY) {
    // 找到下一状态的最大Q值
    double maxQNext = ql->qTable[nextX][nextY][0];
    int a;
    for (a = 1; a < ACTIONS; a++) {
        if (ql->qTable[nextX][nextY][a] > maxQNext) {
            maxQNext = ql->qTable[nextX][nextY][a];
        }
    }
    
    // Q-learning更新公式
    ql->qTable[x][y][action] += ql->alpha * (reward + ql->gamma * maxQNext - ql->qTable[x][y][action]);
}

// 训练智能体
void train(QLearner* ql, int startX, int startY, int goalX, int goalY) {
    // 初始化Q表
    initializeQTableWithHeuristic(ql, goalX, goalY);
    
    int episode;
    for (episode = 0; episode < EPISODES; episode++) {
        int x = startX, y = startY;
        bool done = false;
        int steps = 0;
        
        while (!done && steps < MAX_EPISODE_STEPS) {
            // 选择动作
            int action = chooseAction(ql, x, y, episode);
            
            // 执行动作
            int nextX = x + directions[action][0];
            int nextY = y + directions[action][1];
            
            // 检查是否有效移动
            if (!isValid(nextX, nextY)) {
                // 碰撞惩罚
                updateQValue(ql, x, y, action, -10, x, y);
                done = true;
            } else {
                // 计算奖励
                int reward = -1;  // 每步时间成本
                if (nextX == goalX && nextY == goalY) {
                    reward = 100;  // 到达目标奖励
                    done = true;
                }
                
                // 更新Q值
                updateQValue(ql, x, y, action, reward, nextX, nextY);
                
                // 移动到下一状态
                x = nextX;
                y = nextY;
            }
            
            steps++;
        }
        
        // 每100个episode打印一次进度
        if ((episode + 1) % 100 == 0) {
            printf("Episode %d completed with %d steps\n", episode + 1, steps);
        }
    }
}

// 使用训练好的Q表找到路径
void findPath(QLearner* ql, int startX, int startY, int goalX, int goalY) {
    int x = startX, y = startY;
    bool visited[ROWS][COLS] = {false};
    int path[ROWS*COLS][2];
    int pathLength = 0;
    
    path[pathLength][0] = x;
    path[pathLength][1] = y;
    visited[x][y] = true;
    pathLength++;
    
    while (x != goalX || y != goalY) {
        // 选择Q值最大的动作
        int bestAction = 0;
        double maxQ = ql->qTable[x][y][0];
        
        int a;
        for (a = 1; a < ACTIONS; a++) {
            if (ql->qTable[x][y][a] > maxQ) {
                maxQ = ql->qTable[x][y][a];
                bestAction = a;
            }
        }
        
        // 执行动作
        int nextX = x + directions[bestAction][0];
        int nextY = y + directions[bestAction][1];
        
        // 如果下一步无效或已访问，可能陷入局部最优，回溯
        if (!isValid(nextX, nextY) || visited[nextX][nextY]) {
            printf("No valid path found! (可能陷入局部最优)\n");
            return;
        }
        
        // 记录路径
        path[pathLength][0] = nextX;
        path[pathLength][1] = nextY;
        visited[nextX][nextY] = true;
        pathLength++;
        
        // 移动到下一状态
        x = nextX;
        y = nextY;
    }
    
    // 打印路径
    printf("找到路径，共%d步:\n", pathLength);
    int i,j;
    for (i = 0; i < pathLength; i++) {
        printf("(%d,%d) ", path[i][0], path[i][1]);
        // 在迷宫中标记路径
        maze[path[i][0]][path[i][1]] = 2;
    }
    printf("\n");
    
    // 显示迷宫和路径
    printf("迷宫路径可视化:\n");
    for (i = 0; i < ROWS; i++) {
        for (j = 0; j < COLS; j++) {
            if (maze[i][j] == 0) printf("0 ");
            else if (maze[i][j] == 1) printf("# ");
            else if (maze[i][j] == 2) printf("* ");
        }
        printf("\n");
    }
}

int main() {
    srand(time(NULL));
    
    // 起点和终点
    int startX = 0, startY = 0;
    int goalX = ROWS - 1, goalY = COLS - 1;
    
    // 创建并训练Q学习智能体
    QLearner* ql = createQLearner(0.1, 0.9, 0.3);
    train(ql, startX, startY, goalX, goalY);
    
    // 使用训练好的Q表找到路径
    findPath(ql, startX, startY, goalX, goalY);
    
    free(ql);
    return 0;
}
