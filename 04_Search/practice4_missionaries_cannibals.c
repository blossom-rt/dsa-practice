#include <stdio.h>
#include <stdlib.h>

// 定义状态结构体
typedef struct State {
    int left_missionaries;  // 左岸传教士数量
    int left_cannibals;     // 左岸野人数量
    int right_missionaries; // 右岸传教士数量
    int right_cannibals;    // 右岸野人数量
    int boat_position;      // 船的位置，0 表示左岸，1 表示右岸
    struct State *parent;   // 指向父状态的指针，用于回溯路径
} State;

// 定义已访问状态链表节点
typedef struct VisitedNode {
    State state;
    struct VisitedNode *next;
} VisitedNode;

// 全局已访问状态链表头指针
VisitedNode *visited_head = NULL;

// 检查状态是否合法
int is_valid(State *state) {
    // 两岸传教士数量不能为负，野人数量也不能为负
    if (state->left_missionaries < 0 || state->left_cannibals < 0 ||
        state->right_missionaries < 0 || state->right_cannibals < 0) {
        return 0;
    }
    // 任何时刻，两岸的野人数量不能多于传教士数量（传教士数量为 0 时除外）
    if ((state->left_missionaries != 0 && state->left_cannibals > state->left_missionaries) ||
        (state->right_missionaries != 0 && state->right_cannibals > state->right_missionaries)) {
        return 0;
    }
    return 1;
}

// 检查状态是否已访问
int is_visited(State *state) {
    VisitedNode *current = visited_head;
    while (current != NULL) {
        if (current->state.left_missionaries == state->left_missionaries &&
            current->state.left_cannibals == state->left_cannibals &&
            current->state.right_missionaries == state->right_missionaries &&
            current->state.right_cannibals == state->right_cannibals &&
            current->state.boat_position == state->boat_position) {
            return 1;
        }
        current = current->next;
    }
    return 0;
}

// 添加状态到已访问链表
void add_visited(State *state) {
    VisitedNode *new_node = (VisitedNode *)malloc(sizeof(VisitedNode));
    new_node->state = *state;
    new_node->next = visited_head;
    visited_head = new_node;
}

// 释放已访问链表内存
void free_visited() {
    VisitedNode *current = visited_head;
    while (current != NULL) {
        VisitedNode *temp = current;
        current = current->next;
        free(temp);
    }
    visited_head = NULL;
}

// 检查是否达到目标状态
int is_goal(State *state) {
    return state->left_missionaries == 0 && state->left_cannibals == 0;
}

// 打印状态
void print_state(State *state) {
    printf("左岸: 传教士 %d 人，野人 %d 人；右岸: 传教士 %d 人，野人 %d 人；船在 %s岸\n",
           state->left_missionaries, state->left_cannibals,
           state->right_missionaries, state->right_cannibals,
           state->boat_position == 0 ? "左" : "右");
}

// 打印路径
void print_path(State *state) {
    if (state->parent != NULL) {
        print_path(state->parent);
    }
    print_state(state);
}

// 深度优先搜索
State *dfs(State *current) {
    if (is_goal(current)) {
        return current;
    }
    
    // 标记当前状态为已访问
    add_visited(current);
    
    // 生成所有可能的下一步状态
    int missionaries_move, cannibals_move;
    for (missionaries_move = 0; missionaries_move <= 2; missionaries_move++) {
        for (cannibals_move = 0; cannibals_move <= 2; cannibals_move++) {
            // 船必须至少载 1 人，且最多载 2 人
            if (missionaries_move + cannibals_move == 0 || missionaries_move + cannibals_move > 2) {
                continue;
            }
            State *next = (State *)malloc(sizeof(State));
            if (current->boat_position == 0) {  // 船在左岸，移动到右岸
                next->left_missionaries = current->left_missionaries - missionaries_move;
                next->left_cannibals = current->left_cannibals - cannibals_move;
                next->right_missionaries = current->right_missionaries + missionaries_move;
                next->right_cannibals = current->right_cannibals + cannibals_move;
                next->boat_position = 1;
            } else {  // 船在右岸，移动到左岸
                next->left_missionaries = current->left_missionaries + missionaries_move;
                next->left_cannibals = current->left_cannibals + cannibals_move;
                next->right_missionaries = current->right_missionaries - missionaries_move;
                next->right_cannibals = current->right_cannibals - cannibals_move;
                next->boat_position = 0;
            }
            next->parent = current;
            
            // 检查新状态是否合法且未被访问
            if (is_valid(next) && !is_visited(next)) {
                State *result = dfs(next);
                if (result != NULL) {
                    return result;
                }
            }
            free(next);
        }
    }
    return NULL;
}

int main() {
    // 初始状态：3 个传教士、3 个野人在左岸，船在左岸
    State initial = {3, 3, 0, 0, 0, NULL};
    State *solution = dfs(&initial);
    
    if (solution != NULL) {
        printf("找到过河方案：\n");
        print_path(solution);
    } else {
        printf("没有找到过河方案！\n");
    }
    
    // 释放已访问链表内存
    free_visited();
    
    return 0;
}
