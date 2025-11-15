#include<stdlib.h>
#include<unistd.h>
#include <sys/types.h>
#include<stdio.h>
#include<sys/wait.h>

int main(){
    pid_t id = fork();
    if(id == 0){
        int cnt = 10;
        while(cnt){
            printf("child process:%d,parent process:%d,cnt:%d\n",getpid(),getppid(),cnt--);
            sleep(1);
        }
        exit(0);
    }
    sleep(15);
    pid_t ret = wait(NULL);
    if(id > 0){
        printf("wait sucess:%d\n",ret);

    }
}