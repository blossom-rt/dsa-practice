#include <stdio.h>
int main ()
{
  FILE * pFile;
  int c;
  int n = 0;
  pFile = fopen ("file.txt", "r");
  if (pFile == NULL) perror ("Error opening file"); // 打开失败
  else
  {
    while ((c = fgetc (pFile)) != EOF) // 获取一个字符，并判断是否到达文件末尾
    {
      if (c == 'd') n++; // 统计字符 'd' 在文件中出现的次数
    }
    fclose (pFile); // 一定记得要关闭文件
    printf ("The file contains %d 'd' characters.\n", n);
  }
  return 0;
}
