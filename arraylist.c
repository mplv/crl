#include <stdlib.h>
#include "arraylist.h"

void AL_Resize(ArrayList *l)
{
    // make a copy of the pointer
    ArrayList *tmp = l;
    // create a new arraylist that has twice the cap
    l = malloc(sizeof(ArrayList));
    // twice the cap
    l->cap = tmp->cap * 2;
    // keep the same addpos
    l->addPos = tmp->addPos;
    // recreate the data holding array
    l->arr = calloc(l->cap,sizeof(void *));
    // copy over the data
    int i = 0;
    for (i = 0; i < tmp->cap; i++) {
        l->arr[i] = tmp->arr[i];
    }
    // free the old list
    free(tmp->arr);
    free(tmp);
}

void AL_Destroy(ArrayList *l)
{
    if (AL_Size(l) == 0)
    {
        free(l->arr);
        free(l);
    }
}

ArrayList* AL_New()
{
    // create a new list
    ArrayList *l = malloc(sizeof(ArrayList));
    // start it at 20 cap
    l->cap = 20;
    // create the data holding array
    l->arr = calloc(l->cap,sizeof(void *));
    l->addPos = 0;
    return l;
}

int AL_Size(ArrayList *l)
{
    // 1 higher than the actual size like java
    return l->addPos;
}

void AL_Add(ArrayList *l, void* data)
{
    // if we are at the end of the array resize
    if (l->addPos == l->cap)
    {
        AL_Resize(l);
    }
    // now add it
    l->arr[l->addPos++] = data;
    
}

void* AL_RemoveLast(ArrayList *l)
{
    if (AL_Size(l) > 0)
    {
        void *data = l->arr[l->addPos-1];
        l->arr[l->addPos-1] = NULL;
        l->addPos--;
        return data;
    }
    return NULL;
}

void* AL_Remove(ArrayList *l, int pos)
{
    // as long as the pos to remove is less than the cap and add pos
    if (pos < l->cap && pos < l->addPos) {
        // create a copy of the data pointer
        void *data = l->arr[pos];
        // for each element after move the element down by one
        int i = pos;
        for (i = pos; i < l->addPos; i++) {
            // if the next position is less than the cap
            if (i+1 < l->cap) {
                l->arr[i] = l->arr[i+1];
            } else { //otherwise
                l->arr[i] = NULL;
                l->addPos -= 1;
            }
        }
        // return the data or NULL
        return data;
    }
    return NULL;
}

