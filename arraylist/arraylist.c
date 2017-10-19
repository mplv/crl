#include <stdlib.h>
#include "arraylist.h"

static void resize(ArrayList *l)
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
int ListDestroy(ArrayList *l)
{
    // as long as there are 0 elements
    if (ListSize(l) == 0)
    {
        // free the array
        free(l->arr);
        // also free the list
        free(l);
        return 0;
    }
    return 1;
}

void* ListGet(ArrayList *l, int pos)
{
    // because AL_Size returns an int 1 higher than the next empty pos
    // this check makes sure that the pos is indeed populated with an element
    if (pos > -1 && ListSize(l) > pos)
    {
        // return that element
        void *ret = l->arr[pos];
        return ret;
    }
    // otherwise we hope you dont deref this result without checking
    return NULL;
}

void* ListPeek(ArrayList *l)
{
	if (l->addPos > 0) {
		return l->arr[l->addPos-1];
	}
	return NULL;
}

ArrayList* NewList()
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

int ListSize(ArrayList *l)
{
    // 1 higher than the actual size like java
    return l->addPos;
}

void ListAdd(ArrayList *l, void* data)
{
    // if we are at the end of the array resize
    if (l->addPos == l->cap)
    {
        resize(l);
    }
    // now add it
    l->arr[l->addPos] = data;
    l->addPos += 1;

}

void ListPush(ArrayList *l, void* data) {
	ListAdd(l,data);
}

void* ListRemoveLast(ArrayList *l)
{
    // as long as there are elements
    if (ListSize(l) > 0)
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

void* ListPop(ArrayList *l){
	return ListRemoveLast(l);
}

void* ListRemove(ArrayList *l, int pos)
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
            }
        }
	// dec the add position so that size works correctly
	l->addPos -= 1;
        // return the data or NULL
        return data;
    }
    return NULL;
}
