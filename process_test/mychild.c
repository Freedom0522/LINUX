#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<assert.h>
#include<string.h>

#define NUM 10
typedef void(*func_t)();//函数指针

func_t handlerTask[NUM];//函数指针数组


void task1()
{
    printf("handler task1\n");
}

void task2()
{
    printf("handler task2\n");
}

void task3()
{
    printf("handler task3\n");
}

// typedef void (*func_t)();

void loadTask()//回调函数
{
    memset(handlerTask,0,sizeof(handlerTask));//初始化函数指针数组
    handlerTask[0]=task1;//任务函数的指针填进去
    handlerTask[1]=task2;
    handlerTask[2]=task3;
}

int main()
{
    pid_t id = fork();//创建子进程
    assert(id != -1);
    if(id == 0)
    {
        int cnt = 5;
        while(cnt)
        {
            printf("child running,pid:%d,ppid:%d,cnt:%d\n",getpid(),getppid(),cnt--);//子进程进行打印操作
            sleep(1);
        }//
        exit(0);
    }//
    //1.os释放子进程的僵尸状态
    //2.获取子进程的退出结果
    //等待期间，子进程没有退出的时候，父进程只能阻塞等待
    int status = 0;//退出码
    // int ret = waitpid(id,&status,0);
    // if(ret>0)
    // {
    //     printf("wait success,exit code:%d,sig:%d\n",(status>>8)&0XFF,status& 0x7F);
    // }
    // else
    // {
    //     printf("child exit ")
    // }
    loadTask();
    while(1)
    {
        // pid_t ret = waitpid(id+1,&status,WNOHANG);//?
        pid_t ret = waitpid(id,&status,WNOHANG);
        if(ret == 0)
        {
            printf("wait done,but child is running.... parent running other things\n");
            for(int i = 0;handlerTask[i]!=NULL;i++)
            {
                handlerTask[i]();//采用回调执行我们想让父进程在空闲的时候做的事情
            }
        }
        else if(ret > 0)
        {
            printf("wait:sucess,exit code: %d.sig: %d\n",(status>>8)&0XFF,status& 0x7F);
            break;
        }
        else
        {
            printf("waitpid call failed\n");//waitpid调用失败，id错误

        }
        sleep(1);//子进程一直在进行循环在循环期间父进程一直执行回调任务一直到子进程结束，而且父进程执行几次任务与子进程循环次数无关
    }
    return 0;
}
