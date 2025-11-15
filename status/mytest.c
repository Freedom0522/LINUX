#include<stdio.h>
#include<unistd.h>
#include <sys/types.h>

int global_value = 100;


int main(){
    pid_t id = fork();
    if(id < 0){
        printf("fork error\n");
        return 1;
    }
    else if(id == 0){
        int cnt = 0;
        while(1){
            printf("child process,pid:%d,ppid:%d | global_value:%d,&global_value:%p\n",getpid(),getppid(),global_value,&global_value);
            sleep(1);
            cnt++;
            if(cnt == 10){
                global_value = 300;
                printf("子进程已经更改了全局变量...........\n");
            }
        }
        printf("child\n");
    }
    else{
        printf("parent process,pid:%d,ppid:%d | global_value:%d,&global_value:%p\n",getpid(),getppid(),global_value,&global_value);
        sleep(2);
        printf("father\n");
    }
    
}