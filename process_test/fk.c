#include<stdio.h>
#include<unistd.h>

int main(){
    pid_t id = fork();
    if(id <0){
        perror("fork");
        return 1;
    }
    else if(id == 0){
        while(1)
        {
            printf("我是子进程,pid: %d,ppid: %d\n",getpid(),getppid());
            sleep(1);
        }
    }
    else
    {
        //parent
        while(2)
        {
            printf("我是父进程,pid:%d,ppid:%d\n",getpid(),getppid());
            sleep(3);
        }
    }
}