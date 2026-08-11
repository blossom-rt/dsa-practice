#include<stdio.h>
#include<stdlib.h>

//定义树的结构 
typedef struct TreeNode{
	int data;
	struct TreeNode* parent;
	struct TreeNode* firstChild;
	struct TreeNode* nextSibling;
}TreeNode;

//定义动态数组结构
typedef struct{
	int* data;
	int capacity;
	int size;
}DynamicArray;

//函数声明
TreeNode* createNode(int data, TreeNode* parent);
int countNodes(TreeNode* root);
DynamicArray* findNodePath(TreeNode* root, int target);
void freeArray(DynamicArray* arr);

//创建新节点
TreeNode* createNode(int data, TreeNode* parent){
	TreeNode* node=(TreeNode*)malloc(sizeof(TreeNode));
	if(!node){
		fprintf(stderr,"内存分配失败\n");
		exit(EXIT_FAILURE); 
	}
	node->data = data;
	node->parent = parent;
	node->firstChild = NULL;
	node->nextSibling = NULL;
	return node;
}

//计算树的节点总数
int countNodes(TreeNode* root){
	if(!root) return 0;
	int count=1;
	TreeNode* child = root->firstChild;
	while(child){
		count += countNodes(child);
		child = child->nextSibling;
	}
	return count;
} 

//查找从根到目标节点的路径
DynamicArray* findNodePath(TreeNode* root, int target){
	if(!root) return NULL;
    
    //找到目标节点
	if (root->data == target) {
        DynamicArray* path = (DynamicArray*)malloc(sizeof(DynamicArray));
        if (!path) {
            fprintf(stderr, "内存分配失败\n");
            exit(EXIT_FAILURE);
        }
        path->size = 1;
        path->capacity = 10;
        path->data = (int*)malloc(path->capacity * sizeof(int));
        if (!path->data) {
            fprintf(stderr, "内存分配失败\n");
            exit(EXIT_FAILURE);
        }
        path->data[0] = root->data;
        return path;
    }
    
    // 递归查找子节点
    TreeNode* child = root->firstChild;
    while (child) {
        DynamicArray* path = findNodePath(child, target);
        if (path) {
            // 将当前节点添加到路径开头
            if (path->size >= path->capacity) {
                path->capacity *= 2;
                path->data = (int*)realloc(path->data, path->capacity * sizeof(int));
                if (!path->data) {
                    fprintf(stderr, "内存分配失败\n");
                    exit(EXIT_FAILURE);
                }
            }
            // 后移元素
            int i; 
            for (i = path->size; i > 0; i--) {
                path->data[i] = path->data[i-1];
            }
            path->data[0] = root->data;
            path->size++;
            return path;
        }
        child = child->nextSibling;
    }
    
    return NULL;  // 未找到
} 

//释放动态数组内存
void freeArray(DynamicArray* arr) {
    if (arr) {
        if (arr->data) free(arr->data);
        free(arr);
    }
}

// 释放树内存（递归释放）
void freeTree(TreeNode* root) {
    if (!root) return;
    freeTree(root->firstChild);
    freeTree(root->nextSibling);
    free(root);
} 

int main(){
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
