#include "myStdio.h"
#include<stdio.h>

int main()
{
    FILE_ *fp  = fopen_("./log.txt","w");
    if(fp == NULL)
    {
        return 1;
    }
    int cnt = 10;
    // const char* msg = "hello world\n";
    const char* msg = "hello world";
    while(1)
    {
        
        fwrite_(msg,strlen(msg),fp);
        fflush(fp);//立马把数据刷新出来
        sleep(1);
        printf("count: %d\n",cnt);
        if(cnt == 5)fflush_(fp);
        cnt--;
        if(cnt == 0)break;
        
    }//现象？
    fclose_(fp);
    //>log.txt清空文件
    }
    