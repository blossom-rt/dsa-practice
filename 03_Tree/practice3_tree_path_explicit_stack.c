#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// 动态数组结构定义
typedef struct {
    int* data;
    int size;
    int capacity;
} DynamicArray;

// 树节点结构定义
typedef struct TreeNode {
    int data;
    struct TreeNode* firstChild;
    struct TreeNode* nextSibling;
    struct TreeNode* parent;  // 新增父指针，加速路径回溯
} TreeNode;

// 动态数组操作
DynamicArray* createArray() {
    DynamicArray* arr = (DynamicArray*)malloc(sizeof(DynamicArray));
    arr->data = NULL;
    arr->size = 0;
    arr->capacity = 0;
    return arr;
}

void arrayAppend(DynamicArray* arr, int value) {
    if (arr->size >= arr->capacity) {
        arr->capacity = arr->capacity ? arr->capacity * 2 : 10;
        arr->data = (int*)realloc(arr->data, arr->capacity * sizeof(int));
    }
    arr->data[arr->size++] = value;
}

void freeArray(DynamicArray* arr) {
    free(arr->data);
    free(arr);
}

// 创建新节点（带父指针）
TreeNode* createNode(int data, TreeNode* parent) {
    TreeNode* node = (TreeNode*)malloc(sizeof(TreeNode));
    node->data = data;
    node->firstChild = NULL;
    node->nextSibling = NULL;
    node->parent = parent;  // 记录父节点
    return node;
}

// 递归查找节点（更简洁的实现）
TreeNode* findNode(TreeNode* root, int target) {
    if (!root) return NULL;
    if (root->data == target) return root;
    
    // 先检查子节点
    TreeNode* child = findNode(root->firstChild, target);
    if (child) return child;
    
    // 再检查兄弟节点
    return findNode(root->nextSibling, target);
}

// 构建从节点到根的路径
DynamicArray* buildPath(TreeNode* node) {
    DynamicArray* path = createArray();
    TreeNode* current = node;
    
    while (current) {
        arrayAppend(path, current->data);
        current = current->parent;
    }
    
    // 反转路径以获得从根到目标的顺序
    int i; 
	for (i = 0; i < path->size / 2; i++) {
        int temp = path->data[i];
        path->data[i] = path->data[path->size - i - 1];
        path->data[path->size - i - 1] = temp;
    }
    
    return path;
}

// 高性能路径查找函数
DynamicArray* findNodePath(TreeNode* root, int target) {
    TreeNode* node = findNode(root, target);
    if (!node) return NULL;
    
    return buildPath(node);
}

// 非递归节点计数（使用栈）
int countNodes(TreeNode* root) {
    if (!root) return 0;
    
    // 使用动态扩容的栈
    TreeNode** stack = NULL;
    int capacity = 100;
    int top = -1;
    
    stack = (TreeNode**)malloc(capacity * sizeof(TreeNode*));
    stack[++top] = root;
    
    int count = 0;
    
    while (top >= 0) {
        TreeNode* current = stack[top--];
        count++;
        
        // 动态扩容栈
        if (top + 10 >= capacity) {
            capacity *= 2;
            stack = (TreeNode**)realloc(stack, capacity * sizeof(TreeNode*));
        }
        
        // 将子节点压入栈
        TreeNode* child = current->firstChild;
        while (child) {
            stack[++top] = child;
            child = child->nextSibling;
        }
    }
    
    free(stack);
    return count;
}

// 释放树内存
void freeTree(TreeNode* root) {
    if (!root) return;
    freeTree(root->firstChild);
    freeTree(root->nextSibling);
    free(root);
}

// 主函数示例
int main() {
    // 构建示例树（带父指针）
    TreeNode* root = createNode(1, NULL);
    root->firstChild = createNode(2, root);
    root->firstChild->nextSibling = createNode(3, root);
    root->firstChild->nextSibling->nextSibling = createNode(4, root);
    
    root->firstChild->firstChild = createNode(5, root->firstChild);
    root->firstChild->firstChild->nextSibling = createNode(6, root->firstChild);
    
    root->firstChild->nextSibling->nextSibling->firstChild = createNode(7, root->firstChild->nextSibling->nextSibling);
    
    // 测试节点计数
    printf("树的节点总数: %d\n", countNodes(root));
    
    // 测试查找节点路径
    int target = 6;
    DynamicArray* path = findNodePath(root, target);
    
    if (path) {
        printf("从根节点到节点 %d 的路径: ", target);
        int i;
		for (i = 0; i < path->size; i++) {
            printf("%d ", path->data[i]);
        }
        printf("\n");
        freeArray(path);
    } else {
        printf("未找到节点 %d\n", target);
    }
    
    // 释放树内存
    freeTree(root);
    
    return 0;
}
