#include "invertedIndex.h"
TfIdfList newTfIdfListNode (char *filename, double tfidf_sum);
TfIdfList insertTfIdfListNode (TfIdfList list, TfIdfList node);
void printTfIdfList(TfIdfList list);
TfIdfList insertMerge (TfIdfList list, TfIdfList node);