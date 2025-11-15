#include<stdio.h>
#include<unistd.h>

int main()
{
    chdir("./home/whb");
    while(1)
    {
        printf("我是一个进程：%d\n",getpid());
        sleep(1);
    }
}