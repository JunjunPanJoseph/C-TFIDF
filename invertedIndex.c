#include "invertedIndex.h"
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "InvertedIndexBST.h"
#include "FileList.h"
#include <Math.h>
#include "TfIdfList.h"
struct wordList{
    char *word;
    int count;
    struct wordList * next;
};
struct wordList *addWord(struct wordList *list,  char *word){
    //add word count to the list of word
    if (list == NULL){
        struct wordList *tmp = malloc(sizeof(struct wordList));
        tmp->count = 1;
        tmp->word = word;
        tmp->next = NULL;
        return tmp;
    }
    if (strcmp(list->word, word) == 0){
        list->count ++;
        free(word);
    } else {
        list->next = addWord(list->next, word);
    }
    return list;
}
void printWordList (struct wordList * list){
    if (list == NULL){
        printf("NULL");
    } else {
        printf("[%s, %d]->", list->word, list->count);
        printWordList(list->next);
    }
}

void freeWordList(struct wordList *list){
    //free the struct without release memory
    if (list == NULL) return;
    freeWordList(list->next);
    free(list);
}
char * normaliseWord(char *str){
    if (str == NULL) return str;
    int L = strlen(str);
    int j = 0;
    int start = 1;
    for (int i = 0; i < L; i++){
        char currStr = str[i];
        if (currStr == ' ' && start){
            //Remove space in the fount
            continue;
        }
        if (currStr != ' '){
            start = 0;
        }
        //Change to lower case
        str[j++] = tolower(str[i]);
    }
    int i = strlen(str) - 1;
    //Remove space in the end
    while (i > 0 && str[i] == ' '){
        str[i--] = '\0';
    }
    //Remove last char
    if (i > 0 && (
        str[i] == '.' ||
        str[i] == ',' ||
        str[i] == ';' ||
        str[i] == '?'
        )){
        str[i] = '\0';
    }
    return str;
}
InvertedIndexBST generateInvertedIndex(char *collectionFilename){
    InvertedIndexBST retBST = NULL;
    FILE *fp = fopen(collectionFilename, "r");
    if (fp == NULL) 
        return retBST;
    char *fileNameBuffer = malloc(1 * 100);
    int n;
    struct wordList *tmpList = NULL;

    while ((n = fscanf(fp, "%s", fileNameBuffer)) != EOF){
        //open the file
        FILE *currFile = fopen(fileNameBuffer, "r");
        char *newWord = malloc(1 * 100);
        int total = 0;
        //get the words
        while (fscanf(currFile, "%s", newWord) != EOF){
            total++;
            normaliseWord(newWord);
            //printf("%s\n", newWord);
            tmpList = addWord(tmpList, newWord);
            newWord = malloc(1 * 100);
        }
        //printWordList(tmpList);
        //printf("\n");
        struct wordList *curr = tmpList;
        while (curr != NULL){
            //insert new node to tree
            retBST = insertInvertedIndexBST(retBST, curr->word, newNode(fileNameBuffer, ((double) curr->count) / total));
            curr = curr->next;
        }
        //free the struct without free word
        freeWordList(tmpList);
        tmpList = NULL;
        fileNameBuffer = malloc(1 * 100);
    }
    return retBST;
}
void recPrintInvertedIndex(InvertedIndexBST tree, FILE *fp, int print){
    //print the answer
    if (tree == NULL)
        return;
    //print left first
    recPrintInvertedIndex(tree->left, fp, print);
    fprintf(fp, "%s", tree->word);
    FileList curr = tree->fileList;
    while(curr != NULL){
       fprintf(fp, " %s", curr->filename);
       if (print){
            fprintf(fp, ",%lf", curr->tf);
       }
        curr = curr->next;
    }
    fprintf(fp, "\n");
    recPrintInvertedIndex(tree->right, fp, print);
    return;
}
void printInvertedIndex(InvertedIndexBST tree){
    FILE *fp = fopen("invertedIndex.txt", "w+");
    recPrintInvertedIndex(tree, fp, 0);
}

TfIdfList calculateTfIdf(InvertedIndexBST tree, char *searchWord , int D){
    if (tree == NULL){
        return NULL;
    }
    //recursively find the node which contain the word
    if (strcmp (searchWord, tree->word) < 0){
        return calculateTfIdf(tree->left, searchWord , D);
    } else if (strcmp (searchWord, tree->word) > 0){
        return calculateTfIdf(tree->right, searchWord , D);
    } else {
        TfIdfList retList = NULL;
        FileList currFile = tree->fileList;
        if (currFile == NULL) return NULL;
        //printf("divide: %d/%d = %lf\n", getFileListLen(currFile), D, D/((double) getFileListLen(currFile) ));
        //calculate idf
        double idf = log10(((double) getFileListLen(currFile) )/ D);
        //printf("idf = %lf\n", idf);
        while(currFile != NULL){
            //calculate tfidf for every file that contains the word
            TfIdfList node = newTfIdfListNode (currFile->filename, currFile->tf * idf);
            retList =  insertTfIdfListNode(retList, node);
            currFile = currFile->next;
        }
        return retList;
    }
}

TfIdfList retrieve(InvertedIndexBST tree, char* searchWords[] , int D){
    TfIdfList mergeList = NULL;
    TfIdfList retList = NULL;
    int i = 0;
    TfIdfList curr;
    TfIdfList tmp;
    while (searchWords[i] != NULL){
        char *currChar = searchWords[i];
        TfIdfList currList = calculateTfIdf(tree, currChar , D);
        curr = currList;
        while(curr != NULL){
            tmp = curr->next;
            curr->next = NULL;
            mergeList =  insertMerge(mergeList, curr);
            curr = tmp;
        }
        i++;
    }

    curr = mergeList;
    while(curr != NULL){
        tmp = curr->next;
        curr->next = NULL;
        retList =  insertTfIdfListNode(retList, curr);
        curr = tmp;
    }
    return retList;
}

void testNormaliseWord(){
    char testS1[20] = "     .Net     ";
    char testS2[20] = " smh.com.au ";
    char testS3[20] = "Sydney!";
    char testS4[20] = "why?";
    char testS5[20] = "order.";
    char testS6[20] = "text;";
    char testS7[20] = "abc.net.au.";
    char testS8[20] = "sydney???";
    printf("%s", normaliseWord(testS1));
    printf("%s\n", testS1);
    printf("%s", normaliseWord(testS2));
    printf("%s\n", testS2);
    printf("%s\n", normaliseWord(testS3));
    printf("%s\n", testS3);
    printf("%s\n", normaliseWord(testS4));
    printf("%s\n", testS4);
    printf("%s\n", normaliseWord(testS5));
    printf("%s\n", testS5);
    printf("%s\n", normaliseWord(testS6));
    printf("%s\n", testS6);
    printf("%s\n", normaliseWord(testS7));
    printf("%s\n", testS7);
    printf("%s\n", normaliseWord(testS8));
    printf("%s\n", testS8);
}
void testInvertedIndexBST(){
    InvertedIndexBST bst = generateInvertedIndex("collection.txt");
    recPrintInvertedIndex(bst, stdout, 1);
}
void testCalculateTfIdf(){
    InvertedIndexBST bst = generateInvertedIndex("test.txt");
    //recPrintInvertedIndex(bst, stdout, 1);
    int D = 2;
    printf("#nodes: %d\n", D);
    printf("Word: [this]\n");

    TfIdfList list1 = calculateTfIdf(bst, "this" , D);

    printTfIdfList(list1);
    printf("Word: [example]\n");
    TfIdfList list2 = calculateTfIdf(bst, "example" , D);
    printTfIdfList(list2);
}
void testRetrieve(){
    InvertedIndexBST bst = generateInvertedIndex("test.txt");
    //recPrintInvertedIndex(bst, stdout, 1);
    int D = 2;
    printf("#nodes: %d\n", D);
    printf("Word: [this, example]\n");
    char *words[] = {"this", "example", NULL};
    TfIdfList list = retrieve(bst, words , D);
    printTfIdfList(list);
}
int main(void){
    //testNormaliseWord();
    //testInvertedIndexBST();
    testCalculateTfIdf();
    printf("=====    Retrieve   =====\n");
    testRetrieve();
    return 0;
}