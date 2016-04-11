#include <stdlib.h>
#include <stdio.h>
#include "sorted-index.h"

SortedIndexPtr SICreate(CompareFuncT cf, DestructFuncT df)
{
    SortedIndexPtr list = (SortedIndexPtr)malloc(sizeof(SortedIndexPtr));
    list->head = NULL;
    list->compare = cf;
    list->destroy = df;
    return list;
}

void SIDestroy(SortedIndexPtr list)
{
    ListNodePtr currentNode = list->head;
    ListNodePtr prevNode = list->head;

    ListNodePtr currentFileNode = list->head;
    ListNodePtr prevFileNode = list->head;

    //Check if list is empty
    if(currentNode == NULL)
    {
        return;
    }
    
    //Empty list from tail to head
    while(list->head != NULL)
    {
        while(currentNode->next != NULL)
        {
            prevNode = currentNode;
            currentNode = currentNode->next;   
        }

        list->destroy(currentNode->data);

        if(currentNode == prevNode)
        {
            //Destroy file nodes
            currentFileNode = currentNode->files;
            prevFileNode = currentNode->files;
            while(currentNode->files != NULL)
            {
                while(currentFileNode->next != NULL)
                {
                    prevFileNode = currentFileNode;
                    currentFileNode = currentFileNode->next;
                }

                list->destroy(currentFileNode->data);

                if(currentFileNode == prevFileNode)
                {
                    free(currentNode->files);
                    currentNode->files = NULL;
                }
                else
                {
                    currentFileNode = prevFileNode;
                    free(currentFileNode->next);
                    currentFileNode->next = NULL;
                    currentFileNode = currentNode->files;
                    prevFileNode = currentNode->files;
                }
            }
            free(list->head);
            list->head = NULL;
        }
        else
        {
            //Destroy file nodes
            currentFileNode = currentNode->files;
            prevFileNode = currentNode->files;
            while(currentNode->files != NULL)
            {
                while(currentFileNode->next != NULL)
                {
                    prevFileNode = currentFileNode;
                    currentFileNode = currentFileNode->next;
                }

                list->destroy(currentFileNode->data);

                if(currentFileNode == prevFileNode)
                {
                    free(currentNode->files);
                    currentNode->files = NULL;
                }
                else
                {
                    currentFileNode = prevFileNode;
                    free(currentFileNode->next);
                    currentFileNode->next = NULL;
                    currentFileNode = currentNode->files;
                    prevFileNode = currentNode->files;
                }
            }

            currentNode = prevNode;
            free(currentNode->next);
            currentNode->next = NULL;
            currentNode = list->head;
            prevNode = list->head;
        }
    }

    //Free the list ptr once list is empty
    free(list);
    return;
}

int SIInsert(SortedIndexPtr list, void *word, void *file)
{
    //initialize the new node
    printf("Allocating...\n"); //tag
    ListNodePtr newNode = (ListNodePtr)malloc(sizeof(ListNodePtr));
    newNode->occurences = 0;
    newNode->data = NULL;
    newNode->next = NULL;
    newNode->files = NULL;

    ListNodePtr currentNode = list->head;
    ListNodePtr prevNode = list->head;
    
    //Check if list is empty:
    if(currentNode == NULL)
    { 
        printf("Inserting first node...\n"); //tag
        newNode->data = word;
        list->head = newNode;

        newNode->files = (ListNodePtr)malloc(sizeof(ListNodePtr));
        newNode->files->occurences = 1;
        newNode->files->data = file;
        newNode->files->next = NULL;
        newNode->files->files = NULL;
        return 1;
    }

    printf("Comparing...\n"); //tag
    int compareValue = list->compare(currentNode->data,word);
    
    //Check if newNode belongs in front of list
    if(compareValue > 0)
    {

        printf("Inserting at beginning...\n"); //tag
        newNode->next = list->head;
        list->head = newNode; 

        newNode->data = word;
        newNode->files = (ListNodePtr)malloc(sizeof(ListNodePtr));
        newNode->files->occurences = 1;
        newNode->files->data = file;
        newNode->files->files = NULL;
        return 1;
    }

    //Check if newNode belongs in middle of list
    printf("searching list for word...\n"); //tag
    while(currentNode != NULL)
    {
        compareValue = list->compare(currentNode->data,word);
        printf("currentNode->data: %s\n",currentNode->data); //tag
        printf("compareValue: %d\n",compareValue); //tag
        if(compareValue < 0)
        {
            prevNode = currentNode;
            currentNode = currentNode->next;
        }
        else if(compareValue == 0)
        {
            printf("found word searching records...\n"); //tag
            prevNode = currentNode;
            currentNode = currentNode->files;
            
            compareValue = list->compare(currentNode->data,file);
            if(compareValue > 0)
            {
                printf("Inserting record at beginning...\n"); //tag
                prevNode->files = newNode;
                newNode->next = currentNode;
                
                newNode->occurences = 1;
                newNode->data = file;
                return 1;
            }
            while(currentNode != NULL)
            {
                compareValue = list->compare(currentNode->data,file);
                if(compareValue < 0)
                {
                    prevNode = currentNode;
                    currentNode = currentNode->next;
                }
                else if(compareValue == 0)
                {
                    printf("found record adding occurence...\n"); //tag
                    currentNode->occurences++;
                    return 1;
                }
                else if(compareValue > 0)
                {
                    printf("Inserting record in middle...\n"); //tag
                    prevNode->next = newNode;
                    newNode->next = currentNode;
                    
                    newNode->occurences = 1;
                    newNode->data = file;
                    return 1;
                }
            }
            printf("adding to end of record list...\n"); //tag
            currentNode->next = newNode;
            newNode->occurences = 1;
            newNode->data = file;
            return 1;
            
        }
        else if(compareValue > 0)
        {
            printf("Inserting word in middle...\n"); //tag
            prevNode->next = newNode;
            newNode->next = currentNode;

            newNode->data = word;
            newNode->files = (ListNodePtr)malloc(sizeof(ListNodePtr));
            newNode->files->occurences = 1;
            newNode->files->data = file;
            newNode->files->files = NULL;
            return 1;
        }
    }

    //newNode belongs at end of list
    printf("Adding word to end of list...\n"); //tag
    prevNode->next = newNode;

    newNode->data = word;
    newNode->files = (ListNodePtr)malloc(sizeof(ListNodePtr));
    newNode->files->occurences = 1;
    newNode->files->data = file;
    newNode->files->files = NULL;
    return 1;
}


//Prints list assuming data is of type int (for testing).
void SIPrintList(SortedIndexPtr list)
{
    ListNodePtr wordNode = list->head;
    ListNodePtr fileNode = wordNode->files;

    printf("\n{\"list\":[\n");
    while(wordNode != NULL)
    {
        fileNode = wordNode->files;
        printf("\t{\"%s\":[\n",wordNode->data);
        while(fileNode != NULL)
        {
            printf("\t\t{\"%s\":%d}\n",fileNode->data,fileNode->occurences);
            fileNode = fileNode->next;
        }
        printf("\t]}\n");
        wordNode = wordNode->next;
    }
    printf("]}\n\n");
    return;
}
