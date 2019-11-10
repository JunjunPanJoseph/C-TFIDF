#include "TfIdfList.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
TfIdfList newTfIdfListNode (char *filename, double tfidf_sum){
    TfIdfList node = malloc(sizeof *node);
    node->next = NULL;
    node->tfidf_sum = tfidf_sum;
    node->filename = filename;
    return node;
}
TfIdfList insertTfIdfListNode (TfIdfList list, TfIdfList node){
    if (list == NULL){
        return node;
    } else {
        if (list->tfidf_sum > node->tfidf_sum){
            list->next = insertTfIdfListNode(list->next, node);
            return list;
        } else if  (list->tfidf_sum < node->tfidf_sum){
            node->next = list;
            return node;
        } else {
            if (strcmp(node->filename, list->filename) < 0){
                node->next = list;
                return node;
            } else {
                list->next =  insertTfIdfListNode (list->next, node);
                return list;
            }
        }
    }
}
TfIdfList insertMerge (TfIdfList list, TfIdfList node){
    if (list == NULL){
        return node;
    } else {
        if (strcmp(node->filename, list->filename) == 0){
            list->tfidf_sum += node->tfidf_sum;
            free(node);
            return list;
        } else {
            list->next =  insertMerge (list->next, node);
            return list;
        }
    }
}
void printTfIdfList(TfIdfList list){
    if (list == NULL) {
        printf("NULL\n");
        return;
    }
    printf("[%s, %lf]->", list->filename, list->tfidf_sum);
    printTfIdfList(list->next);
}