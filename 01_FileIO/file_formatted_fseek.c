#include <stdio.h>
#include <stdlib.h> // exit 函数需要的头文件

int main()
{
    FILE *fp; 
    int i, n;
    // 以读写模式打开文件，不存在则创建，存在则清空
    if ((fp = fopen("abc", "w+")) == NULL) 
    {
        printf("can't open abc file\n");
        exit(0);
    }
    // 写入 1 - 10，每个占 3 字符宽度
    for (i = 1; i < 11; i++) 
    {
        fprintf(fp, "%3d", i);
    }
    // 按特定偏移读取并输出
    for (i = 0; i < 10; i++) 
    {
        // 从文件开头，每次偏移 i*3 字节
        fseek(fp, i * 3L, SEEK_SET); 
        fscanf(fp, "%d", &n);
        printf("%3d", n);
    }
    fclose(fp);
    return 0;
}
