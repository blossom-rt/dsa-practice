#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 九宫格大小
#define ROW 3
#define COL 3
#define SIZE ROW*COL
#define MAX_DEPTH 1000

// 目标状态
int target[SIZE] = {1, 2, 3, 8, 0, 4, 7, 6, 5};

// 方向数组：上、右、下、左
int dirs[4][2] = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};
char dirNames[4] = {'U', 'R', 'D', 'L'};  // 方向名称

// 计算曼哈顿距离
int manhattanDistance(int* state) {
    int distance = 0,i;
    for (i = 0; i < SIZE; i++) {
        if (state[i] == 0) continue;
        
        int target_pos;
        for (target_pos = 0; target_pos < SIZE; target_pos++) {
            if (target[target_pos] == state[i]) break;
        }
        
        int cur_row = i / COL;
        int cur_col = i % COL;
        int tar_row = target_pos / COL;
        int tar_col = target_pos % COL;
        
        distance += abs(cur_row - tar_row) + abs(cur_col - tar_col);
    }
    return distance;
}

// 检查两个状态是否相同
int isSameState(int* a, int* b) {
	int i;
    for (i = 0; i < SIZE; i++) {
        if (a[i] != b[i]) return 0;
    }
    return 1;
}

// 打印状态
void printState(int* state) {
	int i,j;
    for (i = 0; i < ROW; i++) {
        for (j = 0; j < COL; j++) {
            printf("%d ", state[i * COL + j]);
        }
        printf("\n");
    }
    printf("********************\n");
}

// 交换数组中两个位置的值
void swap(int* arr, int i, int j) {
    int temp = arr[i];
    arr[i] = arr[j];
    arr[j] = temp;
}

// 递归DFS搜索
int dfs(int* state, int g, int bound, int zero_pos, int* path, int prev_dir) {
    // 新增：检查递归深度，防止栈溢出
    if (g >= MAX_DEPTH - 1) {
        return MAX_DEPTH;
    }
    
    int h = manhattanDistance(state);
    int f = g + h;
    
    if (f > bound) return f;  // 返回最小的超出bound的值
    
    if (h == 0) {  // 找到目标状态
        path[g] = -1;  // 路径结束标记
        return -1;     // 表示已找到解
    }
    
    int min = MAX_DEPTH;
    
    // 尝试四个方向
    int i;
    for (i = 0; i < 4; i++) {
        // 避免回退到父节点（防止来回移动）
        if ((prev_dir != -1) && (i + prev_dir == 2)) continue;
        
        int zero_row = zero_pos / COL;
        int zero_col = zero_pos % COL;
        
        int new_row = zero_row + dirs[i][0];
        int new_col = zero_col + dirs[i][1];
        
        if (new_row >= 0 && new_row < ROW && new_col >= 0 && new_col < COL) {
            int new_zero_pos = new_row * COL + new_col;
            
            // 生成新状态
            swap(state, zero_pos, new_zero_pos);
            
            // 递归搜索
            path[g] = i;  // 记录当前方向
            int t = dfs(state, g + 1, bound, new_zero_pos, path, i);
            
            if (t == -1) return -1;  // 找到解
            if (t < min) min = t;
            
            // 回溯
            swap(state, zero_pos, new_zero_pos);
        }
    }
    
    return min;
}

// IDA* 主函数
int idaStar(int* start, int* path) {
    int bound = manhattanDistance(start);
    
    while (1) {
        int zero_pos;
        for (zero_pos = 0; zero_pos < SIZE; zero_pos++) {
            if (start[zero_pos] == 0) break;
        }
        
        // 复制初始状态，避免修改原数组
        int state[SIZE];
        memcpy(state, start, SIZE * sizeof(int));
        
        int t = dfs(state, 0, bound, zero_pos, path, -1);
        if (t == -1) return bound;  // 找到解
        if (t == MAX_DEPTH) return -1;  // 无解
        
        bound = t;  // 调整bound为最小的超出值
    }
}

// 打印路径
void printPath(int* start, int* path) {
    printf("IDA* 搜索路径:\n");
    printState(start);
    
    int current[SIZE];
    memcpy(current, start, SIZE * sizeof(int));
    
    int i = 0;
    while (path[i] != -1 && i < MAX_DEPTH - 1) {  // 防止越界
        int dir = path[i];
        int zero_pos;
        
        for (zero_pos = 0; zero_pos < SIZE; zero_pos++) {
            if (current[zero_pos] == 0) break;
        }
        
        int zero_row = zero_pos / COL;
        int zero_col = zero_pos % COL;
        
        int new_row = zero_row + dirs[dir][0];
        int new_col = zero_col + dirs[dir][1];
        int new_zero_pos = new_row * COL + new_col;
        
        swap(current, zero_pos, new_zero_pos);
        printf("移动方向: %c\n", dirNames[dir]);
        printState(current);
        
        i++;
    }
    
    printf("路径长度: %d\n", i);
}

int main() {
    // 初始状态
    int start[SIZE] = {2, 0, 3, 1, 6, 4, 8, 7, 5};
    
    printf("初始状态:\n");
    printState(start);
    
    printf("目标状态:\n");
    printState(target);
    
    // 存储路径
    int path[MAX_DEPTH];
    
    // 使用IDA*搜索
    printf("\n执行IDA*搜索...\n");
    int path_length = idaStar(start, path);
    
    if (path_length >= 0) {
        printPath(start, path);
    } else {
        printf("未找到路径或超出最大深度!\n");
    }
    
    return 0;
}
