#include"process.h"

char style[S_NUM] = {'-','.' ,'#' ,'>' ,'+'};


void ProcessOn(){//函数定义

    int cnt = 0;
    char bar[NUM];
    memset(bar,'\0',sizeof(bar));
    //reverse
    const char *label = "|\\-/";
    //101 times
    while(cnt <= 100){
        // printf("[%-100s][%d%%][%c]\r",bar,cnt,label[cnt%4]); //预留空间
        printf("[%-100s][%d%%][%c]\r",bar,cnt,label[cnt%4]);
        fflush(stdout);//让数据从缓冲区刷新出来
        bar[cnt++] = style[N];
        usleep(50000);

    }
    printf("\n"); 
}