#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<assert.h>
#include<string.h>
#include<aio.h>

#define NUM 1024
#define OPT_NUM 64
// #define DEBUG
char lineCommand[NUM];
char* myargv[OPT_NUM];//指针数组
int lastCode = 0;
int lastSig = 0;

int main()
{
    while(1)
    {
    //输入提示符
    printf("用户名@主机名 当前路径# ");
    fflush(stdout);
    //获取输入,输入的时候，自带'\n'
    char *s = fgets(lineCommand,sizeof(lineCommand)-1,stdin);//从终端读入，预留出‘\0'的空间
    assert(s!=NULL);
    (void)s;
    //清楚最后一个'\n'
    lineCommand[strlen(lineCommand)-1]=0;
    // printf("test: %s\n",lineCommand);
    //字符串切割，ls -a -l -i ->"ls","-a","-l","-i
    myargv[0] = strtok(lineCommand," ");
    int i = 1;
    if(myargv[0]!=NULL&&strcmp(myargv[0],"ls")== 0)
    {
        myargv[i++]=(char*)"--color=auto";
    }
    
    //如果没有字串了，strtok->NULL,myargv[end]=NULL
    // int i = 1;
    while(myargv[i++] = strtok(NULL," "));
    //执行cd时更改子进程目录，父进程目录没有更改，因此执行pwd目录没有更改
    //如果是cd命令，不需要创建子进程，让shell自己执行对应的程序，本质就是执行系统接口
    //不需要让子进程执行的命令，而是让shell自己执行的命令->内建命令
    if(myargv[0]!=NULL&&strcmp(myargv[0],"cd") == 0)
    {
        if(myargv[1]!=NULL)chdir(myargv[1]);
        continue;
    }
    if(myargv[0] != NULL&&myargv[1]!=NULL&&strcmp(myargv[0],"echo")==0)
    {
        if(strcmp(myargv[1],"$?")==0)
        {
            printf("%d,%d\n",lastCode,lastSig);
        }
        else
        {
            printf("%s\n",myargv[1]);
        }
        continue;
    }
#ifdef DEBUG// 测试是否成功，条件编译
    for(int i = 0;myargv[i];i++)
    {
        printf("myargv[%d]:%s\n",i,myargv[i]);
    }
#endif

    //执行命令
    pid_t id = fork();
    assert(id != -1);
    if(id == 0)
    {
        execvp(myargv[0],myargv);
        exit(1);
    }
    int status = 0;
    pid_t ret = waitpid(id,&status,0);
    assert(ret>0);
    (void)ret;//ret定义没被使用，忽略警告
    lastCode = (status>>8)&0xFF;
    lastSig = status &0x7F;
    
    }
}