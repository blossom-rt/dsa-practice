#include <stdio.h>
#include <stdlib.h>

// 定义树节点结构
typedef struct TreeNode {
    int data;                   // 节点数据
    struct TreeNode* parent;    // 父节点指针
    struct TreeNode* firstChild; // 第一个子节点指针
    struct TreeNode* nextSibling; // 下一个兄弟节点指针
} TreeNode;

// 创建新节点
TreeNode* createNode(int data, TreeNode* parent) {
    TreeNode* node = (TreeNode*)malloc(sizeof(TreeNode));
    if (node == NULL) {
        fprintf(stderr, "内存分配失败\n");
        exit(EXIT_FAILURE);
    }
    node->data = data;
    node->parent = parent;
    node->firstChild = NULL;
    node->nextSibling = NULL;
    return node;
}

// 判断节点是否存在于树中
int isNodeExists(TreeNode* root, int target) {
    if (root == NULL) {
        return 0;  // 空树，节点不存在
    }
    
    // 当前节点就是目标节点
    if (root->data == target) {
        return 1;
    }
    
    // 递归检查所有子节点
    TreeNode* child = root->firstChild;
    while (child != NULL) {
        if (isNodeExists(child, target)) {
            return 1;  // 在子树中找到目标节点
        }
        child = child->nextSibling;  // 检查下一个兄弟节点
    }
    
    return 0;  // 未找到目标节点
}

// 释放树的内存
void freeTree(TreeNode* root) {
    if (root == NULL) {
        return;
    }
    
    // 递归释放所有子节点
    TreeNode* child = root->firstChild;
    while (child != NULL) {
        TreeNode* next = child->nextSibling;
        freeTree(child);
        child = next;
    }
    
    // 释放当前节点
    free(root);
}

int main() {
    // 构建示例树
    TreeNode* root = createNode(1, NULL);
    
    // 第一层子节点
    TreeNode* child2 = createNode(2, root);
    TreeNode* child3 = createNode(3, root);
    TreeNode* child4 = createNode(4, root);
    
    // 设置根节点的子节点
    root->firstChild = child2;
    child2->nextSibling = child3;
    child3->nextSibling = child4;
    
    // 第二层子节点
    TreeNode* child5 = createNode(5, child2);
    TreeNode* child6 = createNode(6, child2);
    TreeNode* child7 = createNode(7, child4);
    
    // 设置child2的子节点
    child2->firstChild = child5;
    child5->nextSibling = child6;
    
    // 设置child4的子节点
    child4->firstChild = child7;
    
    // 测试节点存在性
    int testValues[] = {1, 6, 7, 10};
    int numTests = sizeof(testValues) / sizeof(testValues[0]);
    
    int i; 
    for (i = 0; i < numTests; i++) {
        int value = testValues[i];
        if (isNodeExists(root, value)) {
            printf("节点 %d 存在于树中\n", value);
        } else {
            printf("节点 %d 不存在于树中\n", value);
        }
    }
    
    // 释放树的内存
    freeTree(root);
    
    return 0;
}
