#include <stdio.h>
#include <stdlib.h>

// 加密函数
void encryptFile(const char *inputPath, const char *outputPath) {
    FILE *inputFile = fopen(inputPath, "r");
    FILE *outputFile = fopen(outputPath, "w");
    if (inputFile == NULL || outputFile == NULL) {
        perror("文件打开失败");
        return;
    }

    char ch;
    while ((ch = fgetc(inputFile)) != EOF) {
        if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z')) {
            // 判断是大写还是小写字母
            int offset = (ch >= 'a') ? 'a' : 'A';
            // 加密：后移 4 位，环形处理
            ch = (ch - offset + 4) % 26 + offset;
        }
        fputc(ch, outputFile);
    }

    fclose(inputFile);
    fclose(outputFile);
}

// 解密函数
void decryptFile(const char *inputPath, const char *outputPath) {
    FILE *inputFile = fopen(inputPath, "r");
    FILE *outputFile = fopen(outputPath, "w");
    if (inputFile == NULL || outputFile == NULL) {
        perror("文件打开失败");
        return;
    }

    char ch;
    while ((ch = fgetc(inputFile)) != EOF) {
        if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z')) {
            // 判断是大写还是小写字母
            int offset = (ch >= 'a') ? 'a' : 'A';
            // 解密：前移 4 位，环形处理（+22 等价于 -4，避免负数）
            ch = (ch - offset + 22) % 26 + offset;
        }
        fputc(ch, outputFile);
    }

    fclose(inputFile);
    fclose(outputFile);
}

int main() {
    // 测试加密和解密
    const char *originalPath = "original.txt";
    const char *encryptedPath = "encrypted.txt";
    const char *decryptedPath = "decrypted.txt";

    // 写入原始内容到 original.txt
    FILE *originalFile = fopen(originalPath, "w");
    if (originalFile != NULL) {
        fprintf(originalFile, "Hello World! 12345");
        fclose(originalFile);
    }

    // 加密
    encryptFile(originalPath, encryptedPath);
    printf("文件加密完成，加密后文件：%s\n", encryptedPath);

    // 解密
    decryptFile(encryptedPath, decryptedPath);
    printf("文件解密完成，解密后文件：%s\n", decryptedPath);

    return 0;
}
