#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 字符移位加密（加密/解密通用，通过 flag 判断：1 加密，0 解密）
char shiftCrypto(char c, int key, int flag) {
    if (!flag) key = -key;
    return (c + key + 256) % 256;
}

// 位运算加密（异或加密）
char xorCrypto(char c, char key) {
    return c ^ key;
}

// 多层加解密通用函数
void multiCrypto(const char *inPath, const char *outPath, char key, int shiftKey, int layer, int isEncrypt) {
    FILE *in = fopen(inPath, "r");
    if (in == NULL) {
        printf("无法打开输入文件: %s\n", inPath);
        exit(1);
    }

    // 读取整个文件到内存缓冲区，后续逐层直接在缓冲区上处理
    fseek(in, 0, SEEK_END);
    long len = ftell(in);
    fseek(in, 0, SEEK_SET);
    char *buf = (char*)malloc(len + 1);
    if (buf == NULL) {
        printf("内存分配失败\n");
        fclose(in);
        exit(1);
    }
    fread(buf, 1, len, in);
    fclose(in);

    // 逐层加密/解密
    int i;
    for (i = 0; i < layer; i++) {
        long j;
        for (j = 0; j < len; j++) {
            if (isEncrypt) {
                // 加密：先移位，再异或
                buf[j] = shiftCrypto(buf[j], shiftKey, 1);
                buf[j] = xorCrypto(buf[j], key);
            } else {
                // 解密是加密的逆过程：先异或还原，再反向移位
                buf[j] = xorCrypto(buf[j], key);
                buf[j] = shiftCrypto(buf[j], shiftKey, 0);
            }
        }
    }

    FILE *out = fopen(outPath, "w");
    if (out == NULL) {
        printf("无法创建输出文件: %s\n", outPath);
        free(buf);
        exit(1);
    }
    fwrite(buf, 1, len, out);
    fclose(out);
    free(buf);
}

int main() {
    char inPath[256], outPath[256];
    char xorKey;
    int shiftKey, layer, isEncrypt;

    // 使用 fgets 读取带空格的路径
    printf("请输入待处理文件名：");
    fgets(inPath, sizeof(inPath), stdin);
    inPath[strcspn(inPath, "\n")] = 0;  // 移除换行符

    printf("请输入输出文件名：");
    fgets(outPath, sizeof(outPath), stdin);
    outPath[strcspn(outPath, "\n")] = 0;

    printf("请输入移位密钥（0~255）：");
    if (scanf("%d", &shiftKey) != 1 || shiftKey < 0 || shiftKey > 255) {
        printf("无效的移位密钥！\n");
        return 1;
    }

    printf("请输入异或密钥（单个字符）：");
    while (getchar() != '\n');  // 清空缓冲区
    scanf("%c", &xorKey);

    printf("请输入加密层数：");
    if (scanf("%d", &layer) != 1 || layer <= 0) {
        printf("无效的加密层数！\n");
        return 1;
    }

    printf("加密（1）还是解密（0）：");
    if (scanf("%d", &isEncrypt) != 1 || (isEncrypt != 0 && isEncrypt != 1)) {
        printf("无效的选择！\n");
        return 1;
    }

    // 验证输入文件是否存在
    FILE *test = fopen(inPath, "r");
    if (test == NULL) {
        printf("错误：输入文件不存在或无法访问: %s\n", inPath);
        return 1;
    }
    fclose(test);

    printf("开始处理...\n");
    multiCrypto(inPath, outPath, xorKey, shiftKey, layer, isEncrypt);
    printf("处理完成！结果已写入 %s\n", outPath);
    
    return 0;
}
