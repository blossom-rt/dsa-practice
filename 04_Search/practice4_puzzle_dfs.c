#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 九宫格大小
#define ROW 3
#define COL 3

// 目标状态
int target[ROW][COL] = {
    {1, 2, 3},
    {8, 0, 4},
    {7, 6, 5}
};

// 为结构体命名
typedef struct Node {
    int state[ROW][COL];
    struct Node *prev;  // 现在类型匹配了
} Node;

// 堆栈结构体（用于非递归 DFS）
typedef struct PuzzleStack {
    Node *data[10000];
    int top;
} PuzzleStack;

// 初始化堆栈
void initStack(PuzzleStack *stack) {
    stack->top = -1;
}

// 判断堆栈是否为空
int isStackEmpty(PuzzleStack *stack) {
    return stack->top == -1;
}

// 入栈操作
void push(PuzzleStack *stack, Node *node) {
    stack->data[++stack->top] = node;
}

// 出栈操作
Node* pop(PuzzleStack *stack) {
    return stack->data[stack->top--];
}

// 判断两个状态是否相同
int isSameState(int a[ROW][COL], int b[ROW][COL]) {
	int i,j;
    for (i = 0; i < ROW; i++) {
        for (j = 0; j < COL; j++) {
            if (a[i][j] != b[i][j]) {
                return 0;
            }
        }
    }
    return 1;
}

// 打印当前状态
void printState(int state[ROW][COL]) {
	int i,j;
    for (i = 0; i < ROW; i++) {
        for (j = 0; j < COL; j++) {
            printf("%d ", state[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

// 复制状态
void copyState(int from[ROW][COL], int to[ROW][COL]) {
	int i,j;
    for (i = 0; i < ROW; i++) {
        for (j = 0; j < COL; j++) {
            to[i][j] = from[i][j];
        }
    }
}

// 寻找空格（0）的位置
void findZero(int state[ROW][COL], int *x, int *y) {
	int i,j;
    for (i = 0; i < ROW; i++) {
        for (j = 0; j < COL; j++) {
            if (state[i][j] == 0) {
                *x = i;
                *y = j;
                return;
            }
        }
    }
}

// 交换两个位置的数字
void swap(int state[ROW][COL], int x1, int y1, int x2, int y2) {
    int temp = state[x1][y1];
    state[x1][y1] = state[x2][y2];
    state[x2][y2] = temp;
}

// 检查状态是否已访问（用于避免重复访问）
int isVisited(Node *path, int state[ROW][COL]) {
    Node *current = path;
    while (current != NULL) {
        if (isSameState(current->state, state)) {
            return 1;
        }
        current = current->prev;  // 类型现在是兼容的
    }
    return 0;
}

// 递归 DFS 实现 - 修改后
int dfsRecursive(int current[ROW][COL], Node *path, int step, int *maxStep) {
    // 判断是否达到目标状态
    if (isSameState(current, target)) {
        // 计算路径长度
        Node *temp = path;
        int pathLength = 0;
        while (temp != NULL) {
            pathLength++;
            temp = temp->prev;
        }
        
        // 创建路径数组存储节点指针
        Node **pathNodes = (Node **)malloc(pathLength * sizeof(Node *));
        temp = path;
        int i;
        for (i = pathLength - 1; i >= 0; i--) {
            pathNodes[i] = temp;
            temp = temp->prev;
        }
        
        // 正序输出路径
        printf("Path:\n");
        for (i = 0; i < pathLength; i++) {
            printState(pathNodes[i]->state);
        }
        printf("Step=%d, Status=1\n", step);
        
        // 释放路径数组
        free(pathNodes);
        return 1;
    }

    // 超过最大步数限制，剪枝
    if (step >= *maxStep) {
        return 0;
    }

    int x, y;
    // 找到空格位置
    findZero(current, &x, &y);

    // 定义四个方向：上、下、左、右
    int dirs[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
    int d;
    for (d = 0; d < 4; d++) {
        int nx = x + dirs[d][0];
        int ny = y + dirs[d][1];
        // 检查新位置是否在九宫格内
        if (nx >= 0 && nx < ROW && ny >= 0 && ny < COL) {
            int newState[ROW][COL];
            // 复制当前状态到新状态
            copyState(current, newState);
            // 交换空格和相邻数字
            swap(newState, x, y, nx, ny);

            // 检查是否已访问过该状态
            if (!isVisited(path, newState)) {
                // 创建新节点保存新状态
                Node *newNode = (Node *)malloc(sizeof(Node));
                copyState(newState, newNode->state);
                newNode->prev = path;  // 类型兼容

                // 递归搜索
                if (dfsRecursive(newState, newNode, step + 1, maxStep)) {
                    return 1;
                }

                // 释放动态分配的节点内存（回溯）
                free(newNode);
            }
        }
    }

    return 0;
}

// 非递归（堆栈）DFS 实现 - 修改后
int dfsStack(int start[ROW][COL], int *maxStep) {
    PuzzleStack stack;
    initStack(&stack);

    // 创建初始节点
    Node *startNode = (Node *)malloc(sizeof(Node));
    copyState(start, startNode->state);
    startNode->prev = NULL;
    push(&stack, startNode);

    int step = 0;
    while (!isStackEmpty(&stack)) {
        Node *currentNode = pop(&stack);
        int current[ROW][COL];
        copyState(currentNode->state, current);

        // 判断是否达到目标状态
        if (isSameState(current, target)) {
            // 计算路径长度
            Node *temp = currentNode;
            int pathLength = 0;
            while (temp != NULL) {
                pathLength++;
                temp = temp->prev;
            }
            
            // 创建路径数组存储节点指针
            Node **pathNodes = (Node **)malloc(pathLength * sizeof(Node *));
            temp = currentNode;
            int i;
            for (i = pathLength - 1; i >= 0; i--) {
                pathNodes[i] = temp;
                temp = temp->prev;
            }
            
            // 正序输出路径
            printf("Path:\n");
            for (i = 0; i < pathLength; i++) {
                printState(pathNodes[i]->state);
            }
            printf("Step=%d, Status=1\n", step);
            
            // 释放路径数组
            free(pathNodes);
            
            // 释放动态分配的节点内存
            Node *freeNode = startNode;
            while (freeNode != NULL) {
                Node *temp = freeNode;
                freeNode = freeNode->prev;
                free(temp);
            }
            return 1;
        }

        // 超过最大步数限制，剪枝
        if (step >= *maxStep) {
            // 释放动态分配的节点内存
            Node *freeNode = startNode;
            while (freeNode != NULL) {
                Node *temp = freeNode;
                freeNode = freeNode->prev;
                free(temp);
            }
            return 0;
        }

        int x, y;
        // 找到空格位置
        findZero(current, &x, &y);

        // 定义四个方向：上、下、左、右
        int dirs[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
        int d;
        for (d = 0; d < 4; d++) {
            int nx = x + dirs[d][0];
            int ny = y + dirs[d][1];
            // 检查新位置是否在九宫格内
            if (nx >= 0 && nx < ROW && ny >= 0 && ny < COL) {
                int newState[ROW][COL];
                // 复制当前状态到新状态
                copyState(current, newState);
                // 交换空格和相邻数字
                swap(newState, x, y, nx, ny);

                // 检查是否已访问过该状态
                if (!isVisited(currentNode, newState)) {
                    // 创建新节点保存新状态
                    Node *newNode = (Node *)malloc(sizeof(Node));
                    copyState(newState, newNode->state);
                    newNode->prev = currentNode;  // 类型兼容
                    push(&stack, newNode);
                }
            }
        }

        step++;
    }

    // 释放动态分配的节点内存
    Node *freeNode = startNode;
    while (freeNode != NULL) {
        Node *temp = freeNode;
        freeNode = freeNode->prev;
        free(temp);
    }
    return 0;
}

int main() {
    // 初始状态
    int start[ROW][COL] = {
        {2, 0, 3},
        {1, 6, 4},
        {8, 7, 5}
    };

    int maxStep = 10000; 

    printf("Search from:\n");
    printState(start);
    printf("to:\n");
    printState(target);

    // 递归 DFS 求解
    printf("Using Recursive DFS:\n");
    Node *startNodeRecursive = (Node *)malloc(sizeof(Node));
    copyState(start, startNodeRecursive->state);
    startNodeRecursive->prev = NULL;
    dfsRecursive(start, startNodeRecursive, 0, &maxStep);
    // 释放递归中动态分配的节点内存
    Node *freeNodeRecursive = startNodeRecursive;
    while (freeNodeRecursive != NULL) {
        Node *temp = freeNodeRecursive;
        freeNodeRecursive = freeNodeRecursive->prev;
        free(temp);
    }

    printf("\n");

    // 堆栈（非递归）DFS 求解
    printf("Using Stack DFS:\n");
    dfsStack(start, &maxStep);

    return 0;
}
