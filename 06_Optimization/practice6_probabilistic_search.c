#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>

#define _USE_MATH_DEFINES
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define DIM 2         // 函数维度
#define MAX_ITER 10000  // 最大迭代次数
#define STEP_SIZE 0.1   // 初始步长
#define MIN_STEP 0.0001 // 最小步长
#define P_EXPLORE 0.2   // 探索概率

// 目标函数：Rastrigin函数 (多峰函数，有多个局部极小值)
double objectiveFunction(double* x) {
    double A = 10;
    double sum = A * DIM;
    int i; 
    for (i = 0; i < DIM; i++) {
        sum += x[i] * x[i] - A * cos(2 * M_PI * x[i]);
    }
    return sum;
}

// 生成随机初始解
void generateInitialSolution(double* x) {
	int i;
    for (i = 0; i < DIM; i++) {
        x[i] = -5.12 + (double)rand() / RAND_MAX * 10.24;
    }
}

// 生成邻居解
void generateNeighbor(double* x, double* neighbor, double step) {
	int i;
    for (i = 0; i < DIM; i++) {
        neighbor[i] = x[i] + (double)rand() / RAND_MAX * 2 * step - step;
        
        // 边界处理
        if (neighbor[i] < -5.12) neighbor[i] = -5.12;
        if (neighbor[i] > 5.12) neighbor[i] = 5.12;
    }
}

// 基于概率的局部优先搜索
void probabilisticLocalSearch(double* bestSolution) {
    double current[DIM];
    double neighbor[DIM];
    double currentValue, neighborValue, bestValue;
    double step = STEP_SIZE;
    
    // 初始化
    generateInitialSolution(current);
    memcpy(bestSolution, current, DIM * sizeof(double));
    currentValue = objectiveFunction(current);
    bestValue = currentValue;
    
    printf("初始解: (");
    int i;
    for (i = 0; i < DIM; i++) {
        printf("%.4f, ", current[i]);
    }
    printf(") 值=%.4f\n", currentValue);
    
    int iter;
    for (iter = 0; iter < MAX_ITER; iter++) {
        // 基于概率决定是探索还是利用
        double randProb = (double)rand() / RAND_MAX;
        
        if (randProb < P_EXPLORE) {
            // 探索：生成更远的随机解
            generateInitialSolution(neighbor);
        } else {
            // 利用：生成当前解附近的邻居
            generateNeighbor(current, neighbor, step);
        }
        
        neighborValue = objectiveFunction(neighbor);
        
        // 接受准则：以一定概率接受更差的解
        if (neighborValue < currentValue || 
            (double)rand() / RAND_MAX < exp((currentValue - neighborValue) / (step * 10))) {
            memcpy(current, neighbor, DIM * sizeof(double));
            currentValue = neighborValue;
            
            // 更新最优解
            if (currentValue < bestValue) {
                memcpy(bestSolution, current, DIM * sizeof(double));
                bestValue = currentValue;
                printf("迭代 %d: 找到更优解 (", iter);
                int i;
                for (i = 0; i < DIM; i++) {
                    printf("%.4f, ", bestSolution[i]);
                }
                printf(") 值=%.4f\n", bestValue);
                
                // 成功找到更优解，增加步长
                step = STEP_SIZE;
            }
        } else {
            // 没有接受解，减小步长
            step *= 0.99;
            if (step < MIN_STEP) step = MIN_STEP;
        }
    }
    
    printf("最终解: (");
    for (i = 0; i < DIM; i++) {
        printf("%.4f, ", bestSolution[i]);
    }
    printf(") 值=%.4f\n", bestValue);
}

int main() {
    srand(time(NULL));
    
    double bestSolution[DIM];
    
    printf("基于概率的局部优先搜索求解多元函数极值\n");
    printf("目标函数: Rastrigin函数\n");
    printf("函数维度: %d\n", DIM);
    printf("探索概率: %.2f\n", P_EXPLORE);
    
    probabilisticLocalSearch(bestSolution);
    
    return 0;
}
