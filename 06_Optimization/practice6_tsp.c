#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define N 10        // 城市数量
#define MAX_ITER 10000  // 最大迭代次数
#define EPSILON 1e-10   // 平方根计算精度

// 城市坐标
typedef struct {
    double x, y;
} City;

City cities[N] = {
    {0, 0}, {1, 5}, {2, 3}, {5, 1}, {6, 4},
    {3, 6}, {7, 2}, {4, 3}, {8, 5}, {9, 0}
};

// 自定义平方根函数（牛顿迭代法）
double my_sqrt(double x) {
    if (x < 0) return 0;  // 处理负数输入
    
    double guess = x;
    double prev_guess;
    
    do {
        prev_guess = guess;
        guess = (guess + x / guess) / 2;
    } while (guess - prev_guess > EPSILON || prev_guess - guess > EPSILON);
    
    return guess;
}

// 自定义平方函数（避免使用pow）
double square(double x) {
    return x * x;
}

// 计算两个城市之间的欧几里得距离
double distance(City a, City b) {
    return my_sqrt(square(a.x - b.x) + square(a.y - b.y));
}

// 计算路径总长度
double pathLength(int* path) {
    double len = 0;
    int i; 
    for (i = 0; i < N - 1; i++) {
        len += distance(cities[path[i]], cities[path[i+1]]);
    }
    len += distance(cities[path[N-1]], cities[path[0]]);  // 返回起点
    return len;
}

// 生成随机初始路径
void generateInitialPath(int* path) {
	int i;
    for (i = 0; i < N; i++) {
        path[i] = i;
    }
    
    // 随机打乱
    for (i = N - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = path[i];
        path[i] = path[j];
        path[j] = temp;
    }
}

// 交换路径中两个位置的城市，生成邻居解
void swapCities(int* path, int i, int j) {
    int temp = path[i];
    path[i] = path[j];
    path[j] = temp;
}

// 局部搜索求解TSP
void tspLocalSearch(int* bestPath) {
    int currentPath[N];
    int tempPath[N];
    double currentLen, tempLen, bestLen;
    
    // 初始化
    generateInitialPath(currentPath);
    memcpy(bestPath, currentPath, N * sizeof(int));
    
    // 正确初始化路径长度
    currentLen = pathLength(currentPath);
    bestLen = currentLen;
    
    printf("初始路径长度: %.2f\n", bestLen);
    
    int iter;
    for (iter = 0; iter < MAX_ITER; iter++) {
        // 随机选择两个城市进行交换
        int i = rand() % N;
        int j;
        do {
            j = rand() % N;
        } while (i == j);
        
        // 生成新路径
        memcpy(tempPath, currentPath, N * sizeof(int));
        swapCities(tempPath, i, j);
        tempLen = pathLength(tempPath);
        
        // 如果新路径更优，则接受
        if (tempLen < currentLen) {
            memcpy(currentPath, tempPath, N * sizeof(int));
            currentLen = tempLen;
            
            // 更新最优解
            if (currentLen < bestLen) {
                memcpy(bestPath, currentPath, N * sizeof(int));
                bestLen = currentLen;
                printf("迭代 %d: 找到更优路径，长度=%.2f\n", iter, bestLen);
            }
        }
    }
    
    printf("最终路径长度: %.2f\n", bestLen);
}

// 打印路径
void printPath(int* path) {
    printf("路径: ");
    int i;
    for (i = 0; i < N; i++) {
        printf("%d -> ", path[i]);
    }
    printf("%d\n", path[0]);  // 返回起点
}

int main() {
    // 初始化随机数种子，使用固定值以便结果可复现
    srand(42);
    
    int bestPath[N];
    
    printf("旅行商问题 (TSP) 局部最优搜索求解\n");
    printf("城市数量: %d\n", N);
    
    tspLocalSearch(bestPath);
    printPath(bestPath);
    
    return 0;
}
