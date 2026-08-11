#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>  

#define MAX_NODES 100
#define INF 999999999

typedef struct {
    int id;
    double x, y;  // 地理位置坐标
    int isPumpingStation;  // 是否为泵站候选点
    double constructionCost;  // 建设成本（百万）
} Node;

typedef struct {
    int from, to;
    double distance;  // 距离（公里）
    double constructionCost;  // 管线建设成本（百万）
    double maintenanceCost;  // 100年运维成本（百万）
} Edge;

typedef struct {
    Node nodes[MAX_NODES];
    Edge edges[MAX_NODES * MAX_NODES];
    int nodeCount;
    int edgeCount;
} Graph;

typedef struct {
    int id;
    double cost;
} PriorityNode;

typedef struct {
    PriorityNode* nodes;
    int size;
    int capacity;
} PriorityQueue;

// 初始化图
void initGraph(Graph* graph) {
    graph->nodeCount = 0;
    graph->edgeCount = 0;
}

// 添加节点
int addNode(Graph* graph, double x, double y, int isPumpingStation, double constructionCost) {
    if (graph->nodeCount >= MAX_NODES) {
        printf("节点数量已达上限！\n");
        return -1;
    }
    Node node;
    node.id = graph->nodeCount;
    node.x = x;
    node.y = y;
    node.isPumpingStation = isPumpingStation;
    node.constructionCost = constructionCost;
    graph->nodes[graph->nodeCount++] = node;
    return node.id;
}

// 添加边
void addEdge(Graph* graph, int from, int to, double distance, double constructionCost, double maintenanceCost) {
    if (graph->edgeCount >= MAX_NODES * MAX_NODES) {
        printf("边数量已达上限！\n");
        return;
    }
    Edge edge;
    edge.from = from;
    edge.to = to;
    edge.distance = distance;
    edge.constructionCost = constructionCost;
    edge.maintenanceCost = maintenanceCost;
    graph->edges[graph->edgeCount++] = edge;
}

// 创建优先队列
PriorityQueue* createPriorityQueue() {
    PriorityQueue* q = (PriorityQueue*)malloc(sizeof(PriorityQueue));
    q->nodes = (PriorityNode*)malloc(MAX_NODES * sizeof(PriorityNode));
    q->size = 0;
    q->capacity = MAX_NODES;
    return q;
}

// 释放优先队列
void destroyPriorityQueue(PriorityQueue* q) {
    free(q->nodes);
    free(q);
}

// 入队
void enqueue(PriorityQueue* q, int id, double cost) {
    int i;
    for (i = q->size - 1; i >= 0; i--) {
        if (q->nodes[i].cost <= cost) break;
        q->nodes[i + 1] = q->nodes[i];
    }
    q->nodes[i + 1].id = id;
    q->nodes[i + 1].cost = cost;
    q->size++;
}

// 出队
int dequeue(PriorityQueue* q) {
    if (q->size == 0) return -1;
    int id = q->nodes[0].id;
    int i; 
    for (i = 0; i < q->size - 1; i++) {
        q->nodes[i] = q->nodes[i + 1];
    }
    q->size--;
    return id;
}

// 判断队列是否为空
bool isEmpty(PriorityQueue* q) {
    return q->size == 0;
}

// 计算两点间距离
double calculateDistance(double x1, double y1, double x2, double y2) {
    return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

// 启发函数（预估剩余路径成本）
double heuristic(Graph* graph, int current, int end) {
    // 使用欧氏距离估算剩余距离
    double dist = calculateDistance(
        graph->nodes[current].x, graph->nodes[current].y,
        graph->nodes[end].x, graph->nodes[end].y
    );
    
    // 假设每公里建设成本和运维成本的平均值
    double avgConstructionCostPerKm = 5.0;  // 百万/公里
    double avgMaintenanceCostPerKm = 2.0;   // 百万/公里
    
    // 泵站建设成本预估（假设每50公里需要一个泵站）
    double estimatedPumpingStations = dist / 50.0;
    double pumpingStationCost = estimatedPumpingStations * 50.0;  // 每个泵站50百万
    
    return dist * (avgConstructionCostPerKm + avgMaintenanceCostPerKm) + pumpingStationCost;
}

// A*算法求最低成本路径
void aStar(Graph* graph, int start, int end, int* path, int* pathLength) {
    double gScore[MAX_NODES];
    double fScore[MAX_NODES];
    int prev[MAX_NODES];
    bool visited[MAX_NODES];
    
    int i;
    for (i = 0; i < graph->nodeCount; i++) {
        gScore[i] = INF;
        fScore[i] = INF;
        prev[i] = -1;
        visited[i] = false;
    }
    
    gScore[start] = 0;
    fScore[start] = heuristic(graph, start, end);
    
    PriorityQueue* q = createPriorityQueue();
    enqueue(q, start, fScore[start]);
    
    while (!isEmpty(q)) {
        int u = dequeue(q);
        
        if (u == end) break;
        
        if (visited[u]) continue;
        visited[u] = true;
        
        // 遍历所有从u出发的边
        for (i = 0; i < graph->edgeCount; i++) {
            if (graph->edges[i].from == u) {
                int v = graph->edges[i].to;
                
                // 计算从起点到v的新成本
                double newG = gScore[u] + 
                             graph->edges[i].constructionCost + 
                             graph->edges[i].maintenanceCost;
                
                // 如果v是泵站，加上泵站建设成本
                if (graph->nodes[v].isPumpingStation) {
                    newG += graph->nodes[v].constructionCost;
                }
                
                if (newG < gScore[v]) {
                    prev[v] = u;
                    gScore[v] = newG;
                    fScore[v] = newG + heuristic(graph, v, end);
                    enqueue(q, v, fScore[v]);
                }
            }
        }
    }
    
    // 构建路径
    *pathLength = 0;
    int current = end;
    while (current != -1) {
        path[(*pathLength)++] = current;
        current = prev[current];
    }
    
    // 反转路径
    for (i = 0; i < (*pathLength) / 2; i++) {
        int temp = path[i];
        path[i] = path[*pathLength - i - 1];
        path[*pathLength - i - 1] = temp;
    }
    
    destroyPriorityQueue(q);
}

// 打印路径和总成本
void printPath(Graph* graph, int* path, int pathLength) {
    printf("管线规划路径: ");
    double totalCost = 0;
    
    int i,j;
    for (i = 0; i < pathLength; i++) {
        printf("%d", path[i]);
        if (graph->nodes[path[i]].isPumpingStation) {
            printf("(泵站)");
        }
        if (i < pathLength - 1) {
            printf(" -> ");
            
            // 查找边，计算总成本
            for (j = 0; j < graph->edgeCount; j++) {
                if (graph->edges[j].from == path[i] && graph->edges[j].to == path[i + 1]) {
                    totalCost += graph->edges[j].constructionCost;
                    totalCost += graph->edges[j].maintenanceCost;
                    break;
                }
            }
            
            // 加上泵站建设成本（如果下一个节点是泵站）
            if (graph->nodes[path[i + 1]].isPumpingStation) {
                totalCost += graph->nodes[path[i + 1]].constructionCost;
            }
        }
    }
    printf("\n");
    
    printf("总成本: %.2f 百万元\n", totalCost);
}

int main() {
    Graph graph;
    initGraph(&graph);
    
    // 创建示例节点（简化版）
    int n1 = addNode(&graph, 0, 0, 1, 50.0);    // 起点（泵站）
    int n2 = addNode(&graph, 100, 0, 1, 50.0);  // 泵站候选点
    int n3 = addNode(&graph, 200, 0, 0, 0.0);   // 普通节点
    int n4 = addNode(&graph, 200, 100, 1, 50.0); // 泵站候选点
    int n5 = addNode(&graph, 100, 100, 0, 0.0);  // 普通节点
    int n6 = addNode(&graph, 0, 100, 1, 50.0);   // 泵站候选点
    int n7 = addNode(&graph, 0, 200, 0, 0.0);    // 普通节点
    int n8 = addNode(&graph, 100, 200, 1, 50.0); // 泵站候选点
    int n9 = addNode(&graph, 200, 200, 1, 50.0); // 终点（泵站）
    
    // 添加边（距离单位：公里，成本单位：百万）
    addEdge(&graph, n1, n2, 100, 500, 200);  // 建设成本5百万/公里，运维成本2百万/公里
    addEdge(&graph, n2, n3, 100, 600, 240);  // 地形复杂，成本较高
    addEdge(&graph, n3, n4, 100, 500, 200);
    addEdge(&graph, n4, n9, 100, 500, 200);
    
    addEdge(&graph, n2, n5, 100, 550, 220);
    addEdge(&graph, n5, n8, 100, 550, 220);
    addEdge(&graph, n8, n9, 100, 500, 200);
    
    addEdge(&graph, n1, n6, 100, 500, 200);
    addEdge(&graph, n6, n7, 100, 500, 200);
    addEdge(&graph, n7, n8, 100, 500, 200);
    
    int path[MAX_NODES];
    int pathLength;
    
    // 管线规划
    printf("=== 西气东输管线规划 ===\n");
    aStar(&graph, n1, n9, path, &pathLength);
    printPath(&graph, path, pathLength);
    
    return 0;
}
