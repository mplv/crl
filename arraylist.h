#ifndef arraylist_h
#define arraylist_h

typedef struct {
    int cap;
    int addPos;
    void **arr;
} ArrayList;

ArrayList* AL_New();
void AL_Add(ArrayList *l, void* data);
void* AL_Remove(ArrayList *l, int pos);
void* AL_RemoveLast(ArrayList *l);
int AL_Size(ArrayList *l);
void AL_Destroy(ArrayList *l);
void* AL_Get(ArrayList *l, int pos);


#endif /* arraylist_h */
