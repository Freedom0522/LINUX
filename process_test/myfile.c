#include<stdio.h>
#include<unistd.h>
#include<string.h>

int main()
{
    printf("hello printf\n");
    fprintf(stdout,"hello fprintf\n");
    const char *fputsString = "hello fputs\n";
    fputs(fputsString,stdout);

    const char *wstring = "hello write\n";
    write(1,wstring,strlen(wstring));

    

    fork();

    return 0;//C语言接口打印两次，缓冲区问题
}