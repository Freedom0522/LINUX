#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<assert.h>
#include<string.h>

#define NUM 1024
#define OPT_NUM 64
// #define DEBUG
char lineCommand[NUM];
char* myargv[OPT_NUM];//指针数组

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
    printf("test: %s\n",lineCommand);
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
    waitpid(id,NULL,0);
    }
}