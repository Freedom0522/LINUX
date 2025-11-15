#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<string.h>

int main()
{
    // close(0);
    // close(2);
    // close(1);
    umask(0);//确保创建的文件权限不被系统默认屏蔽掉。
    // int fd = open("log.txt",O_WRONLY|O_CREAT|O_TRUNC,0666);//可读写，不可执行。
    // int fd = open("log.txt",O_WRONLY|O_CREAT|O_APPEND,0666);//输出重定向
    int fd = open("log.txt",O_RDONLY);
    if(fd<0)
    {
        perror("open");//打印出对应的错误描述。
        return 1;
    }

    dup2(fd,0);//输入重定向

    char line[64];
    while(1)
    {
        printf("> ");
        if(fgets(line,sizeof(line),stdin)== NULL) break;//stdin->0
        printf("%s",line);
    }
    

    // dup2(fd,1);//把1文件描述符指向文件变得和fd文件描述符指向一样对吗
    // // printf("fd: %d\n",fd);
    // fprintf(stdout,"open fd:%d\n",fd);
    
    // const char *msg = "hello";
    // write(1,msg,strlen(msg));

    fflush(stdout);

    close(fd);
    return 0;
}