#include "FileList.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
FileList newNode (char *filename, double tf){
    FileList node = malloc(sizeof *node);
    node->next = NULL;
    node->tf = tf;
    node->filename = filename;
    return node;
}
FileList insertNode (FileList list, FileList node){
    
    if (list == NULL){
        return node;
    } else {
        
        if (strcmp(node->filename, list->filename) < 0){
            node->next = list;
            return node;
        } else {
            list->next =  insertNode (list->next, node);
            return list;
        }
    }
}

int getFileListLen(FileList list){
    if (list == 0) return 0;
    return getFileListLen(list->next) + 1;
}