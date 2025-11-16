#pragma once

#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<string.h>
#include<errno.h>
#include<stdlib.h>
#include<assert.h>

#define SIZE 1024
#define SYNV_NOW  1
#define SYNC_LINE 2
#define SYNC_FULL 4

typedef struct _FILE
{
    int flags;//刷新方式
    int fileno;
    char buffer[SIZE];
    int cap;//buffer总容量
    int size;//buffer当前的使用量
}FILE_;

FILE_ *fopen_(const char *path_name,const char* mode);
void fwrite_(const void *ptr,int num,FILE_ *fp);
void fclose_(FILE_ *fp);
void fflush_(FILE_ *fp);