#include<stdio.h>
#include<stdlib.h>
int main()
{
    char ch;
    FILE* pf = fopen("file.txt", "w");
    if (pf == NULL)
    {
        perror("error opening file");
        exit(0);
    }
    ch = getchar();
    while (ch != '$')
    {
        fputc(ch, pf);
        ch = getchar();
    }
    fclose(pf);
}
