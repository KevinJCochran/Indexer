#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include "sorted-index.h"
#include "index-tokenizer.h"

void dirHandler(char * path, SortedIndexPtr index);
void fileHandler(char * path, char * filename, SortedIndexPtr index);

int compareString(void *a,void *b)
{
    char *x = (char*)a;
    char *y = (char*)b;
    return strcmp(x,y);
}int (*compare)(void*,void*) = compareString;

void destroyString(void *x)
{
    return;
}void (*destroy)(void*) = destroyString;

void dirHandler(char * path, SortedIndexPtr index)
{
    DIR *currentDir = opendir(path);
    struct dirent *temp = readdir(currentDir);
    while(temp != NULL)
    {
        if(temp->d_type == DT_DIR)
        {
            strcat(path,temp->d_name);
            dirHandler(path,index);
        }
        else if(temp->d_type == DT_REG)
        {
            fileHandler(path,temp->d_name,index);
        }
        else
        {
            printf("error\n");
        }

        temp = readdir(currentDir);
    }
    closedir(currentDir);
}

void fileHandler(char * path, char * filename, SortedIndexPtr index)
{
    printf("File found: %s/%s\n",path,filename);
    char * filePath = (char*)calloc(1,strlen(path)+1);
    strcpy(filePath,path);
    strcat(filePath,filename);
    FILE * fd = fopen(filePath,"r");
    if(fd == NULL)
    {
        printf("Error opening file...\n");
        return;
    }
    TokenizerT *tk = TKCreate(fd);
    char *token = TKGetNextToken(tk);
    while(token != NULL)
    {
        token = TKGetNextToken(tk);
        SIInsert(index,token,filename);
    }
}

int main(int argc, char ** argv)
{   
    SortedListIndex index = SICreate(compare,destroy);
    dirHandler(argv[2],index);
    SIPrintList(index);
    return 0;
}
