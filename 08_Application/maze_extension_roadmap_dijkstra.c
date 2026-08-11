#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>  

#define MAX_NODES 100
#define INF 999999

typedef struct {
    int id;
    double x, y;  // 坐标
} Node;

typedef struct {
    int from, to;
    double distance;  // 距离
    double drivingTime;  // 自驾时间
    double walkingTime;  // 步行时间
    int roadType;  // 1=高速, 2=主干道, 3=次干道, 4=人行道
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
int addNode(Graph* graph, double x, double y) {
    if (graph->nodeCount >= MAX_NODES) {
        printf("节点数量已达上限！\n");
        return -1;
    }
    Node node;
    node.id = graph->nodeCount;
    node.x = x;
    node.y = y;
    graph->nodes[graph->nodeCount++] = node;
    return node.id;
}

// 添加边
void addEdge(Graph* graph, int from, int to, double distance, double drivingTime, double walkingTime, int roadType) {
    if (graph->edgeCount >= MAX_NODES * MAX_NODES) {
        printf("边数量已达上限！\n");
        return;
    }
    Edge edge;
    edge.from = from;
    edge.to = to;
    edge.distance = distance;
    edge.drivingTime = drivingTime;
    edge.walkingTime = walkingTime;
    edge.roadType = roadType;
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

// Dijkstra算法求最短路径
void dijkstra(Graph* graph, int start, int end, int mode, int* path, int* pathLength) {
    double dist[MAX_NODES];
    int prev[MAX_NODES];
    bool visited[MAX_NODES];
    
    int i;
    for (i = 0; i < graph->nodeCount; i++) {
        dist[i] = INF;
        prev[i] = -1;
        visited[i] = false;
    }
    
    dist[start] = 0;
    
    PriorityQueue* q = createPriorityQueue();
    enqueue(q, start, 0);
    
    while (!isEmpty(q)) {
        int u = dequeue(q);
        
        if (u == end) break;
        
        if (visited[u]) continue;
        visited[u] = true;
        
        // 遍历所有从u出发的边
        for (i = 0; i < graph->edgeCount; i++) {
            if (graph->edges[i].from == u) {
                int v = graph->edges[i].to;
                double cost;
                
                // 根据出行模式选择不同的代价
                if (mode == 0) {  // 自驾
                    cost = graph->edges[i].drivingTime;
                    // 高速优先
                    if (graph->edges[i].roadType == 1) {
                        cost *= 0.8;  // 高速权重降低
                    }
                } else {  // 步行
                    cost = graph->edges[i].walkingTime;
                    // 人行道优先
                    if (graph->edges[i].roadType == 4) {
                        cost *= 0.8;  // 人行道权重降低
                    }
                }
                
                if (dist[v] > dist[u] + cost) {
                    dist[v] = dist[u] + cost;
                    prev[v] = u;
                    enqueue(q, v, dist[v]);
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

// 打印路径
void printPath(Graph* graph, int* path, int pathLength, int mode) {
    printf("路径: ");
    double totalDistance = 0;
    double totalTime = 0;
    
    int i,j;
    for (i = 0; i < pathLength; i++) {
        printf("%d", path[i]);
        if (i < pathLength - 1) {
            printf(" -> ");
            
            // 查找边，计算总距离和总时间
            for (j = 0; j < graph->edgeCount; j++) {
                if (graph->edges[j].from == path[i] && graph->edges[j].to == path[i + 1]) {
                    totalDistance += graph->edges[j].distance;
                    totalTime += (mode == 0) ? graph->edges[j].drivingTime : graph->edges[j].walkingTime;
                    break;
                }
            }
        }
    }
    printf("\n");
    
    printf("总距离: %.2f 公里\n", totalDistance);
    printf("总时间: %.2f 分钟\n", totalTime);
}

int main() {
    Graph graph;
    initGraph(&graph);
    
    // 创建示例地图节点
    int n1 = addNode(&graph, 0, 0);    // 起点
    int n2 = addNode(&graph, 1, 0);    // 路口1
    int n3 = addNode(&graph, 2, 0);    // 路口2
    int n4 = addNode(&graph, 2, 1);    // 路口3
    int n5 = addNode(&graph, 1, 1);    // 路口4
    int n6 = addNode(&graph, 0, 1);    // 路口5
    int n7 = addNode(&graph, 0, 2);    // 路口6
    int n8 = addNode(&graph, 1, 2);    // 路口7
    int n9 = addNode(&graph, 2, 2);    // 终点
    
    // 添加边（距离单位：公里，时间单位：分钟）
    // 高速/主干道
    addEdge(&graph, n1, n2, 1.0, 1.0, 10.0, 2);
    addEdge(&graph, n2, n3, 1.0, 1.0, 10.0, 1);
    addEdge(&graph, n3, n4, 1.0, 1.0, 10.0, 2);
    addEdge(&graph, n4, n9, 1.0, 1.0, 10.0, 2);
    
    // 次干道
    addEdge(&graph, n2, n5, 1.0, 2.0, 15.0, 3);
    addEdge(&graph, n5, n8, 1.0, 2.0, 15.0, 3);
    addEdge(&graph, n8, n9, 1.0, 2.0, 15.0, 3);
    
    // 人行道
    addEdge(&graph, n1, n6, 1.0, 3.0, 8.0, 4);
    addEdge(&graph, n6, n7, 1.0, 3.0, 8.0, 4);
    addEdge(&graph, n7, n8, 1.0, 3.0, 8.0, 4);
    
    int path[MAX_NODES];
    int pathLength;
    
    // 自驾路径规划
    printf("=== 自驾路径规划 ===\n");
    dijkstra(&graph, n1, n9, 0, path, &pathLength);
    printPath(&graph, path, pathLength, 0);
    
    // 步行路径规划
    printf("\n=== 步行路径规划 ===\n");
    dijkstra(&graph, n1, n9, 1, path, &pathLength);
    printPath(&graph, path, pathLength, 1);
    
    return 0;
}
