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

// 深度优先搜索并显示路径
void dfsPath(TreeNode* root) {
    if (root == NULL) {
        return;
    }
    
    // 访问当前节点（输出数据）
    printf("%d ", root->data);
    
    // 递归访问所有子节点（DFS）
    TreeNode* child = root->firstChild;
    while (child != NULL) {
        dfsPath(child);
        child = child->nextSibling;
    }
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
    
    // 显示DFS路径
    printf("树的DFS遍历路径: ");
    dfsPath(root);
    printf("\n");
    
    // 释放树的内存
    freeTree(root);
    
    return 0;
}
