#include "InvertedIndexBST.h"
#include "FileList.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
InvertedIndexBST newInvertedIndexBST(char *word, FileList node){
    InvertedIndexBST retBST= malloc(sizeof(*retBST));
    if (retBST == NULL)
        return retBST;
    retBST->word = word;
    retBST->fileList = node;
    retBST->left = NULL;
    retBST->right = NULL;
    return retBST;
}

InvertedIndexBST insertInvertedIndexBST(InvertedIndexBST bst, char *word, FileList node){
    if (bst == NULL){
        return newInvertedIndexBST(word, node);
    } else {
        int n = strcmp(word, bst->word);
        if (n < 0){
            bst->left = insertInvertedIndexBST(bst->left, word, node);
        } else if (n > 0) {
            bst->right = insertInvertedIndexBST(bst->right, word, node);
        } else {
            bst->fileList = insertNode (bst->fileList, node);
        }
        return bst;
    }
}
int numNodes(InvertedIndexBST bst){
    if (bst == NULL) return 0;
    return 1 + numNodes(bst->left) + numNodes(bst->right);
}