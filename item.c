#include <stdlib.h>
#include "item.h"

RL_Item *RL_NewItem()
{
    RL_Item *i = calloc(1, sizeof(RL_Item));
    return i;
}

void RL_DestroyItem(RL_Item *i)
{
    free(i);
}
