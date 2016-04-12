#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "sorted-index.h"
#include "index-tokenizer.h"

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
    
    FILE *fd = fopen("text.txt","r");
    if(fd == NULL)
    {
        printf("fd NULL\n");
    }
    TokenizerT *tk = TKCreate(fd);
    if(tk == NULL)
    {
        return -1;
    }
    int(*compare)(void*,void*) = compareString;
    void(*destroy)(void*) = destroyString;
    SortedListPtr myList = SLCreate(compare,destroy);

    printf("Attempting to print...\n");
    printf("inputString: %s\n",tk->inputString);
    char *token = TKGetNextToken(tk);
    while(token != NULL)
    {
        SIInsert(myList,token,text.txt);
        printf("%s\n",token);
        free(token);
        token = TKGetNextToken(tk);
    }
    TKDestroy(tk);
    fclose(fd);
    return 0;
}
