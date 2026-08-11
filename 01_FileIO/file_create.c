#include<stdio.h>
#include<stdlib.h>
int main()
{
    FILE* f;
    f = fopen("file.txt", "w");
    if (f != NULL)
    {
        fputs("fopen example", f);
        fclose(f);
        f=NULL;
    }
    return 0;
}
