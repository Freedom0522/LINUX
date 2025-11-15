#include<stdio.h>
#include<unistd.h>
#include<assert.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<stdlib.h>

// const char *bin = "mybin";

int main()
{
    // printf("processing is running...\n");
    // //只要是函数就可能失败，失败后就不会替换
    // // execl("/usr/bin/ls"/*要执行哪个程序*/,"ls","--color=auto","-a","-l",NULL/*在命令行中怎么执行就怎么传参*/);//all exec* end of NULL
    // execl("/usr/bin/top"/*要执行哪个程序*/,"top",NULL);//exec就是把可执行程序的代码数据覆盖当前进程的物理内存中的代码和数据
    // printf("process running done...\n");//由于程序代码数据已经被覆盖，执行新的代码数据了，因此不会执行printf

    printf("processing is running...\n");
    pid_t id = fork();
    assert(id!=-1);
  
    if(id == 0)
    {
        sleep(1);
        // execl("/usr/bin/ls","ls","-a","-l","--color=auto",NULL);
        // execlp("ls","ls","-a","-l","--color=auto",NULL);//自动循环找路径,两个ls不重复，一个告诉我执行谁，一个告诉我怎么执行
        
        // char *const argv_[] = {
        //     "ls",
        //     "-a",
        //     "-l",
        //     "--color=auto",
        //     NULL
        // };
        // // execv("/usr/bin/ls",argv_);//可以将所有执行参数放入数组中，统一传递，而不使用可变参数
        // execvp("ls",argv_);
        // execl("./mycpp","mycpp",NULL);//调用另一个程序
        char *const envp_[] = 
        {
            (char*)"MYENV=1111333344",
            NULL
        };
        extern char** environ;
        // execle("./mybin","mybin",NULL,envp_);//使用自己的环境变量而不是父进程的环境变量
        putenv((char*)"MYENV=4444333311");//把指定的环境变量导入到系统中，environ指向的环境变量表中
        execle("./mybin","mybin",NULL,environ);
        exit(1);
    }
    int status = 0;
    pid_t ret = waitpid(id,&status,0);
    if(ret>0)printf("wait success:exit code:%d,sig:%d\n",(status>>8)&0xFF,status&0x7F);//子进程的退出码，导致子进程终止的信号

    
} 