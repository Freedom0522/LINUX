#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<assert.h>
#include<string.h>
#include<ctype.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<errno.h>

#define NUM 1024
#define OPT_NUM 64
#define NONE_REDIR 0
#define INPUT_REDIR 1
#define OUTPUT_REDIR 2
#define APPEND_REDIR 3

#define trimSpace(start)      \
    do {                      \
        while (isspace(*start)) start++; \
    } while (0)
// #define DEBUG
char lineCommand[NUM];
char* myargv[OPT_NUM];//指针数组
int lastCode = 0;
int lastSig = 0;

int redirType = NONE_REDIR;
char* redirFile = NULL;

void CommandCheck(char* commands)
{
    assert(commands);
    char* start = commands;
    char* end = commands +strlen(commands);//需不需要+1

    while(start<end)
    {
        if(*start == '>')
        {
            *start = '\0';
            start++;
            if(*start == '>')
            {
                redirType =  APPEND_REDIR;
                start++;
            }
            else
            {
                redirType = OUTPUT_REDIR;
            }
            trimSpace(start);
            redirFile = start;
            break;
        }
        else if(*start == '<')
        {
            *start = '\0';
            start++;
            trimSpace(start);
            //填写重定向信息
            redirType = INPUT_REDIR;
            redirFile = start;
            break;
        }
        
        else
        {
            start++;
        }
    }
}


int main()
{
    while(1)
    {
    redirType = NONE_REDIR;
    redirFile = NULL;
    errno = 0;
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
    //"ls -a -l > myfile.txt"->"ls -a -l -i" "myfile.txt"->
    CommandCheck(lineCommand);
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
// #ifdef DEBUG// 测试是否成功，条件编译
//     for(int i = 0;myargv[i];i++)
//     {
//         printf("myargv[%d]:%s\n",i,myargv[i]);
//     }
// #endif

    //执行命令
    pid_t id = fork();
    assert(id != -1);
    if(id == 0)
    {
        //因为命令是子进程执行的，真正重定向的工作一定是子进程来完成
        //如何重定向，是父进程要给子进程提供信息
        //这里的重定向会影响父进程吗不会，创建子进程时只会拷贝task_struct等进程部分信息，文件部分例如打开的文件并不会被拷贝，
        
        switch(redirType)
        {
            case NONE_REDIR:
            //什么都不做
            break;
            case INPUT_REDIR:
            {
                int fd = open(redirFile,O_RDONLY);
                if(fd < 0)
                {
                    perror("open");
                    exit(errno);
                } 
                //重定向的文件已经成功打开了
                dup2(fd,0);

            }
            break;
            case APPEND_REDIR:
            case OUTPUT_REDIR://用法
            {
                umask(0);
                int flags = O_WRONLY |O_CREAT;
                if(redirType == APPEND_REDIR)flags |= O_APPEND;
                else flags |= O_TRUNC;
                int fd = open(redirFile,flags,0666);
                if(fd< 0)
                {
                    perror("open");
                    exit(errno);
                }
                dup2(fd,1);
               
            }
            break;
            default:
            printf("bug?\n");
            break;
        }
        execvp(myargv[0],myargv);//执行程序替换的时候，会不会影响曾经进程打开的重定向的文件？不会，因为内和数据结构和程序加载是两个模块
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
//创建临时文件，文件权限不符合预期