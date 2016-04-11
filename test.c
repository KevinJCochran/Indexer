#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "sorted-index.h"

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


int main(int argc, char ** argv)
{   
    printf("======Beginning Excution======\n");
    
    SortedIndexPtr index = SICreate(compare,destroy);
    printf("======Finished Creation======\n");

    if(index == NULL)
    {
        printf("malloc fail...\n");
        exit(EXIT_FAILURE);
    }

    printf("1:\n");
    SIInsert(index,"cat","text.txt");
    printf("2:\n");
    SIInsert(index,"dog","text.txt");
    printf("3:\n");
    SIInsert(index,"mouse","animals.txt");
    printf("4:\n");
    SIInsert(index,"mouse","animals.txt");
    printf("5:\n");
    SIInsert(index,"dog","animals.txt");
    printf("6:\n");
    SIInsert(index,"cat","animals.txt");
    printf("7:\n");
    SIInsert(index,"cat","text.txt");
    printf("8:\n");
    printf("======Finished Insertion=====\n");

    SIPrintList(index);

    return 0;
}
