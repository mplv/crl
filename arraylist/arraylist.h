#ifndef arraylist_h
#define arraylist_h

typedef struct {
    int cap;
    int addPos;
    void **arr;
} ArrayList;

// Create an arraylist
ArrayList* NewList();

// add an element to the array list
// note: that the user of this data structure
// must have ownership of the data
void ListAdd(ArrayList *l, void* data);
void ListPush(ArrayList *l, void* data);

// remove an element from position pos in list
void* ListRemove(ArrayList *l, int pos);

// remove the last element in the list
// note: this allows a stack like operation
// of the list
void* ListRemoveLast(ArrayList *l);
void* ListPop(ArrayList *l);

// get the size of the list
int ListSize(ArrayList *l);

// destroy the list structure
int ListDestroy(ArrayList *l);

// get an element at a certain position
void* ListGet(ArrayList *l, int pos);
void* ListPeek(ArrayList *l);

#endif /* arraylist_h */
