#include <stdlib.h>
#include "arraylist.h"

void AL_Resize(ArrayList *l)
{
    // TODO works but could be shortened
    // make a copy of the pointer
    ArrayList *tmp = l;
    // create a new arraylist that has twice the cap
    ArrayList *new = calloc(1, sizeof(ArrayList));
    // twice the cap
    new->cap = tmp->cap * 2;
    // keep the same addpos
    new->addPos = tmp->addPos;
    // recreate the data holding array
    new->arr = calloc(new->cap,sizeof(void *));
    // copy over the data
    int i = 0;
    for (i = 0; i < tmp->cap; i++) {
        new->arr[i] = tmp->arr[i];
    }
    // free the old list
    free(tmp->arr);
    l->cap = new->cap;
    l->arr = new->arr;
    l->addPos = new->addPos;
	free(new);
}

// Remember that you must remove all elements before using this function
// or it does nothing and you leak a bunch of data
void AL_Destroy(ArrayList *l)
{
    // as long as there are 0 elements
    if (AL_Size(l) == 0)
    {
        // free the array
        free(l->arr);
        // also free the list
        free(l);
    }
}

void* AL_Get(ArrayList *l, int pos)
{
    // because AL_Size returns an int 1 higher than the next empty pos
    // this check makes sure that the pos is indeed populated with an element
    if (pos > -1 && AL_Size(l) > pos)
    {
        // return that element
        void *ret = l->arr[pos];
        return ret;
    }
    // otherwise we hope you dont deref this result without checking
    return NULL;
}

ArrayList* AL_New()
{
    // create a new list
    ArrayList *l = calloc(1, sizeof(ArrayList));
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
    l->arr[l->addPos] = data;
    l->addPos += 1;

}

void* AL_RemoveLast(ArrayList *l)
{
    // as long as there are elements
    if (AL_Size(l) > 0)
    {
        // get the last element
        // p.s. l->addPos is pointing to empty pos
        // we need to sub 1 to get last added element
        void *data = l->arr[l->addPos-1];
        // set that position to null
        l->arr[l->addPos-1] = NULL;
        // move the addPos down to the now empty slot
        l->addPos--;
        // return the stuff
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
        // for each element after, move the element down by one
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
