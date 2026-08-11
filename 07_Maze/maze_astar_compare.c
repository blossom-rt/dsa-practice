#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

#define ROWS 10
#define COLS 10

// 提前声明Node为结构体类型
typedef struct Node Node;

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

// 定义Node结构体
struct Node {
    int x, y;
    int g, h, f;
    Node* parent;
};

// 定义优先队列结构体 
typedef struct {
    Node** nodes;
    int size;
    int capacity;
} PriorityQueue;

// 优先队列扩容函数 
void resizeQueue(PriorityQueue* q) {
    int newCapacity = q->capacity * 2;
    Node** temp = (Node**)realloc(q->nodes, newCapacity * sizeof(Node*));
    if (temp == NULL) {
        fprintf(stderr, "内存分配失败！\n");
        exit(EXIT_FAILURE);
    }
    q->nodes = temp;
    q->capacity = newCapacity;
    printf("队列扩容至: %d\n", newCapacity);
}

// 创建优先队列函数 
PriorityQueue* createQueue() {
    PriorityQueue* q = (PriorityQueue*)malloc(sizeof(PriorityQueue));
    q->nodes = (Node**)malloc(100 * sizeof(Node*));
    q->size = 0;
    q->capacity = 100;
    return q;
}

// 销毁优先队列函数 
void destroyQueue(PriorityQueue* q) {
	int i; 
    for (i = 0; i < q->size; i++) {
        free(q->nodes[i]);
    }
    free(q->nodes);
    free(q);
}

// 启发式函数 
int heuristic(int x1, int y1, int x2, int y2) {
    return abs(x1 - x2) + abs(y1 - y2);
}

// 检查坐标有效性函数 
bool isValid(int x, int y) {
    return x >= 0 && x < ROWS && y >= 0 && y < COLS && maze[x][y] == 0;
}

// 创建节点函数 
Node* createNode(int x, int y, int g, int h, Node* parent) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->x = x;
    node->y = y;
    node->g = g;
    node->h = h;
    node->f = g + h;
    node->parent = parent;
    return node;
}

// 入队函数 
void enqueue(PriorityQueue* q, Node* node) {
    if (q->size >= q->capacity) {
        resizeQueue(q);
    }
    
    int i;
    for (i = q->size - 1; i >= 0; i--) {
        if (q->nodes[i]->f <= node->f) break;
        q->nodes[i + 1] = q->nodes[i];
    }
    q->nodes[i + 1] = node;
    q->size++;
}

// 出队函数 
Node* dequeue(PriorityQueue* q) {
    if (q->size == 0) return NULL;
    Node* node = q->nodes[0];
    int i;
    for (i = 0; i < q->size - 1; i++) {
        q->nodes[i] = q->nodes[i + 1];
    }
    q->size--;
    return node;
}

// 检查节点是否在队列中函数 
bool isInQueue(PriorityQueue* q, int x, int y) {
	int i;
    for (i = 0; i < q->size; i++) {
        if (q->nodes[i]->x == x && q->nodes[i]->y == y) return true;
    }
    return false;
}

// 打印带路径的迷宫图
void printMazeWithPath(Node** pathNodes, int pathLength) {
    int mazeCopy[ROWS][COLS],i,j;
    memcpy(mazeCopy, maze, sizeof(maze));
    
    for (i = 0; i < pathLength; i++) {
        int x = pathNodes[i]->x;
        int y = pathNodes[i]->y;
        mazeCopy[x][y] = 2;
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

// 单向A*搜索算法函数 
bool aStar(int startX, int startY, int endX, int endY) {
    if (!isValid(startX, startY) || !isValid(endX, endY)) {
        printf("起点或终点无效！\n");
        return false;
    }
    
    // 创建一个优先队列open 
    PriorityQueue* open = createQueue();
    bool closed[ROWS][COLS] = {false};
    
    // 起点节点的创建 
    Node* start = createNode(startX, startY, 0, heuristic(startX, startY, endX, endY), NULL);
    enqueue(open, start); // 使起点节点入open队列 
    
    // 定义方向数组，表示四个可能的方向（上、下、左、右） 
    int directions[4][2] = {{-1,0}, {1,0}, {0,-1}, {0,1}};
    
    //主循环 
    while (open->size > 0) {
        Node* current = dequeue(open); // 从open中取出当前最优节点 
        
        // 当前节点是终点 
        if (current->x == endX && current->y == endY) {
            // 构建路径，回溯parent链 
            Node* pathNodes[ROWS * COLS];
            int pathLength = 0;
            
            Node* p = current;
            while (p != NULL) {
                pathNodes[pathLength++] = p;
                p = p->parent;
            }
            
            // 反转路径（从起点到终点） 
            int i; 
            for (i = 0; i < pathLength / 2; i++) {
                Node* temp = pathNodes[i];
                pathNodes[i] = pathNodes[pathLength - i - 1];
                pathNodes[pathLength - i - 1] = temp;
            }
            
            // 打印路径并显示 
            printf("A*找到路径！路径长度: %d\n", pathLength);
            printf("路径坐标:\n");
            for (i = 0; i < pathLength; i++) {
                printf("(%d, %d)\n", pathNodes[i]->x, pathNodes[i]->y);
            }
            
            printMazeWithPath(pathNodes, pathLength);
            
            destroyQueue(open); // 销毁open队列 
            return true;
        }
        
        // 当前不是终点，标记为已访问 
        closed[current->x][current->y] = true;
        
        // 不断往里面填充新的搜索可能路径节点（A*搜索算法的核心） 
        int i,j;
        // 遍历四个方向的邻居节点
        for (i = 0; i < 4; i++) {
            int nx = current->x + directions[i][0];
            int ny = current->y + directions[i][1];
            
            // 检查邻居合法性 
            if (!isValid(nx, ny) || closed[nx][ny]) continue;
            
            // 计算代价值并创建新节点 
            int g = current->g + 1;
            Node* neighbor = createNode(nx, ny, g, heuristic(nx, ny, endX, endY), current);
            
            // 判断是否加入open队列 
            if (isInQueue(open, nx, ny)) {
                // 检查是否有更优路径，发现更优路径则更新 
                bool found = false;
                for (j = 0; j < open->size; j++) {
                    if (open->nodes[j]->x == nx && open->nodes[j]->y == ny) {
                        if (g < open->nodes[j]->g) {
                            open->nodes[j]->g = g;
                            open->nodes[j]->f = g + open->nodes[j]->h;
                            open->nodes[j]->parent = current;
                        }
                        found = true;
                        break;
                    }
                }
                if (found) free(neighbor); // 否则释放新节点 
                else enqueue(open, neighbor); // 如果不在open中，直接加入 
            } else {
                enqueue(open, neighbor); // 合法且未访问的邻居节点被加入open列表 
            }
        }
    }
    
    // open队列为空，说明没有找到路径 
    printf("A*未找到路径！\n");
    destroyQueue(open);
    return false;
}

// 双向A*搜索算法函数 
bool bidirectionalAStar(int startX, int startY, int endX, int endY) {
	// 输入合法性判断 
    if (!isValid(startX, startY) || !isValid(endX, endY)) {
        printf("起点或终点无效！\n");
        return false;
    }
    
    // 初始化open列表，一个从起点出发，一个从终点出发 
    PriorityQueue* openForward = createQueue();
    PriorityQueue* openBackward = createQueue();
    
    // 记录已经访问的点 
    Node* closedForward[ROWS][COLS] = {0};
    Node* closedBackward[ROWS][COLS] = {0};
    
    // 起点、终点节点创建 
    Node* start = createNode(startX, startY, 0, heuristic(startX, startY, endX, endY), NULL);
    Node* end = createNode(endX, endY, 0, heuristic(endX, endY, startX, startY), NULL);
    
    // 起点、终点加入两个open列表 
    enqueue(openForward, start);
    enqueue(openBackward, end);
    
    // 定义方向数组（表示上、下、左、右四个方向）和两个搜索方向"相遇"的那个节点 
    int directions[4][2] = {{-1,0}, {1,0}, {0,-1}, {0,1}};
    Node* meetNode = NULL;
    
    //主循环（前后搜索交替） 
    while (openForward->size > 0 && openBackward->size > 0) {
        // 前向搜索一步
        Node* currentF = dequeue(openForward); // 从openForward里取出当前f最小节点 
        if (currentF == NULL) continue;
        closedForward[currentF->x][currentF->y] = currentF; // 标记为已访问 
        
        // 若已被后向搜索访问过，则说明相遇了，准备结束 
        if (closedBackward[currentF->x][currentF->y] != NULL) {
            meetNode = currentF;
            break;
        }
        
        //扩展向前邻居节点 
        int i;
        // 遍历四个方向邻居 
        for (i = 0; i < 4; i++) {
            int nx = currentF->x + directions[i][0];
            int ny = currentF->y + directions[i][1];
            
            // 跳过非法或已访问的邻居 
            if (!isValid(nx, ny)) continue;
            if (closedForward[nx][ny] != NULL) continue;
            
            // 创建邻居节点 
            int g = currentF->g + 1;
            Node* neighbor = createNode(nx, ny, g, heuristic(nx, ny, endX, endY), currentF);
            
            if (!isInQueue(openForward, nx, ny) || g < neighbor->g) {
                enqueue(openForward, neighbor); // 若不在open或找到更优路径，加入 
            } else {
                free(neighbor); // 否则释放内存 
            }
        }
        
        // 后向搜索一步
        // 取出后向当前节点并标记 
        Node* currentB = dequeue(openBackward);
        if (currentB == NULL) continue;
        closedBackward[currentB->x][currentB->y] = currentB;
        
        // 若这个节点也被前向访问过，则相遇 
        if (closedForward[currentB->x][currentB->y] != NULL) {
            meetNode = currentB;
            break;
        }
        
        //扩展向后邻居节点 
        // 遍历四个方向邻居
        for (i = 0; i < 4; i++) {
            int nx = currentB->x + directions[i][0];
            int ny = currentB->y + directions[i][1];
            
            // 跳过非法或已访问的邻居 
            if (!isValid(nx, ny)) continue;
            if (closedBackward[nx][ny] != NULL) continue;
            
            // 创建邻居节点
            int g = currentB->g + 1;
            Node* neighbor = createNode(nx, ny, g, heuristic(nx, ny, startX, startY), currentB);
            
            if (!isInQueue(openBackward, nx, ny) || g < neighbor->g) {
                enqueue(openBackward, neighbor); // 若不在open或找到更优路径，加入
            } else {
                free(neighbor); // 否则释放内存
            }
        }
    }
    
    //找到路径后：拼接路径 
    if (meetNode != NULL) {
        // 构建完整路径
        Node* pathNodes[ROWS * COLS];
        int pathLength = 0;
        
        // 从前向路径构建（向前回溯路径） 
        Node* p = meetNode;
        while (p != NULL) {
            pathNodes[pathLength++] = p;
            p = p->parent;
        }
        
        // 反转前向路径
        int i; 
        for (i = 0; i < pathLength / 2; i++) {
            Node* temp = pathNodes[i];
            pathNodes[i] = pathNodes[pathLength - i - 1];
            pathNodes[pathLength - i - 1] = temp;
        }
        
        // 从后向路径构建
        p = closedBackward[meetNode->x][meetNode->y];
        while (p != NULL) {
            pathNodes[pathLength++] = p;
            p = p->parent;
        }
        
        // 打印路径并退出 
        printf("双向A*找到路径！路径长度: %d\n", pathLength);
        printf("路径坐标:\n");
        for (i = 0; i < pathLength; i++) {
            printf("(%d, %d)\n", pathNodes[i]->x, pathNodes[i]->y);
        }
        
        printMazeWithPath(pathNodes, pathLength);
        
        destroyQueue(openForward);
        destroyQueue(openBackward);
        return true;
    }
    
    // 两个open队列为空，说明没有找到路径
    printf("双向A*未找到路径！\n");
    destroyQueue(openForward);
    destroyQueue(openBackward);
    return false;
}

int main() {
	// 输入起点和终点的坐标 
    int startX = 0, startY = 0;
    int endX = ROWS - 1, endY = COLS - 1;
    
    // 运行单向A*算法 
    printf("A*搜索算法求解老鼠走迷宫问题\n");
    printf("起点: (%d, %d), 终点: (%d, %d)\n\n", startX, startY, endX, endY);
    aStar(startX, startY, endX, endY);
    
    // 运行双向A*算法
    printf("\n双向A*搜索算法求解老鼠走迷宫问题\n");
    printf("起点: (%d, %d), 终点: (%d, %d)\n\n", startX, startY, endX, endY);
    bidirectionalAStar(startX, startY, endX, endY); 
    
    return 0;
}
