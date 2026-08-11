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
    while (c != EOF)
    {
      c = fgetc (pFile); // 获取一个字符
      if (c == 'd') n++; // 统计美元符号 '$' 在文件中出现的次数
    }
    fclose (pFile); // 一定记得要关闭文件
    printf ("The file contains %d dollar sign characters (d).\n",n);
  }
  return 0;
}
