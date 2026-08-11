#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_VERTICES 100

// 定义图的邻接表结构
typedef struct Node {
    int vertex;
    struct Node* next;
} Node;

typedef struct Graph {
    Node* adj[MAX_VERTICES];
    int numVertices;
} Graph;

// 定义堆栈结构
typedef struct Stack {
    int data[MAX_VERTICES];
    int top;
} Stack;

// 创建新节点
Node* createNode(int v) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) {
        fprintf(stderr, "内存分配失败\n");
        exit(EXIT_FAILURE);
    }
    newNode->vertex = v;
    newNode->next = NULL;
    return newNode;
}

// 创建图
Graph* createGraph(int vertices) {
    if (vertices <= 0 || vertices > MAX_VERTICES) {
        fprintf(stderr, "顶点数无效: %d\n", vertices);
        return NULL;
    }
    
    Graph* graph = (Graph*)malloc(sizeof(Graph));
    if (graph == NULL) {
        fprintf(stderr, "内存分配失败\n");
        exit(EXIT_FAILURE);
    }
    
    graph->numVertices = vertices;
    int i;
    for (i = 0; i < vertices; i++) {
        graph->adj[i] = NULL;
    }
    return graph;
}

// 添加边
void addEdge(Graph* graph, int src, int dest) {
    if (graph == NULL) {
        fprintf(stderr, "图未初始化\n");
        return;
    }
    
    if (src < 0 || src >= graph->numVertices || dest < 0 || dest >= graph->numVertices) {
        fprintf(stderr, "顶点索引无效: %d -> %d\n", src, dest);
        return;
    }
    
    Node* newNode = createNode(dest);
    newNode->next = graph->adj[src];
    graph->adj[src] = newNode;
}

// 初始化堆栈
void initStack(Stack* stack) {
    if (stack == NULL) return;
    stack->top = -1;
}

// 检查堆栈是否为空
bool isEmpty(Stack* stack) {
    return stack == NULL || stack->top == -1;
}

// 入栈
bool push(Stack* stack, int item) {
    if (stack == NULL) return false;
    if (stack->top >= MAX_VERTICES - 1) {
        fprintf(stderr, "堆栈溢出\n");
        return false;
    }
    stack->data[++stack->top] = item;
    return true;
}

// 出栈
int pop(Stack* stack) {
    if (isEmpty(stack)) return -1;
    return stack->data[stack->top--];
}

// 获取栈顶元素
int peek(Stack* stack) {
    if (isEmpty(stack)) return -1;
    return stack->data[stack->top];
}

// 递归方法查找路径
bool findPathRecursive(Graph* graph, int src, int dest, bool visited[], int path[], int* pathIndex) {
    if (graph == NULL || pathIndex == NULL) return false;
    if (src < 0 || src >= graph->numVertices || dest < 0 || dest >= graph->numVertices) return false;
    
    visited[src] = true;
    path[(*pathIndex)++] = src;

    if (src == dest) {
        return true;
    }

    Node* current = graph->adj[src];
    while (current != NULL) {
        int neighbor = current->vertex;
        if (!visited[neighbor]) {
            if (findPathRecursive(graph, neighbor, dest, visited, path, pathIndex)) {
                return true;
            }
        }
        current = current->next;
    }

    (*pathIndex)--;
    return false;
}

// 使用递归方法打印路径
void printPathRecursive(Graph* graph, int src, int dest) {
    if (graph == NULL) {
        printf("图未初始化\n");
        return;
    }
    
    if (src < 0 || src >= graph->numVertices || dest < 0 || dest >= graph->numVertices) {
        printf("顶点索引无效\n");
        return;
    }
    
    bool visited[MAX_VERTICES] = {false};
    int path[MAX_VERTICES];
    int pathIndex = 0;

    printf("递归方法: ");
    if (findPathRecursive(graph, src, dest, visited, path, &pathIndex)) {
        printf("找到路径: ");
        int i;
        for (i = 0; i < pathIndex; i++) {
            printf("%d ", path[i]);
        }
        printf("\n");
    } else {
        printf("未找到从 %d 到 %d 的路径\n", src, dest);
    }
}

// 使用堆栈方法查找路径
void printPathStack(Graph* graph, int src, int dest) {
    if (graph == NULL) {
        printf("图未初始化\n");
        return;
    }
    
    if (src < 0 || src >= graph->numVertices || dest < 0 || dest >= graph->numVertices) {
        printf("顶点索引无效\n");
        return;
    }
    
    bool visited[MAX_VERTICES] = {false};
    Stack stack;
    initStack(&stack);
    int parent[MAX_VERTICES];
    int i;
    for (i = 0; i < graph->numVertices; i++) {
        parent[i] = -1;
    }

    push(&stack, src);
    visited[src] = true;
    parent[src] = -1;

    bool pathFound = false;

    while (!isEmpty(&stack)) {
        int current = pop(&stack);

        if (current == dest) {
            pathFound = true;
            break;
        }

        Node* temp = graph->adj[current];
        while (temp != NULL) {
            int neighbor = temp->vertex;
            if (!visited[neighbor]) {
                push(&stack, neighbor);
                visited[neighbor] = true;
                parent[neighbor] = current;
            }
            temp = temp->next;
        }
    }

    printf("堆栈方法: ");
    if (pathFound) {
        printf("找到路径: ");
        Stack pathStack;
        initStack(&pathStack);
        int at = dest;
        while (at != -1) {
            push(&pathStack, at);
            at = parent[at];
        }

        while (!isEmpty(&pathStack)) {
            printf("%d ", pop(&pathStack));
        }
        printf("\n");
    } else {
        printf("未找到从 %d 到 %d 的路径\n", src, dest);
    }
}

// 释放图的内存
void freeGraph(Graph* graph) {
    if (graph == NULL) return;
    
    int i;
    for (i = 0; i < graph->numVertices; i++) {
        Node* current = graph->adj[i];
        while (current != NULL) {
            Node* temp = current;
            current = current->next;
            free(temp);
        }
    }

    free(graph);
}

int main() {
    // 创建图
    Graph* graph = createGraph(5);
    addEdge(graph, 0, 1);
    addEdge(graph, 0, 2);
    addEdge(graph, 1, 2);
    addEdge(graph, 2, 3);
    addEdge(graph, 3, 4);

    // 测试不同路径
    printPathRecursive(graph, 0, 4);
    printPathStack(graph, 0, 4);
    
    printf("\n测试无效路径:\n");
    printPathRecursive(graph, 4, 0);
    printPathStack(graph, 4, 0);

    // 释放内存
    freeGraph(graph);

    return 0;
}    
