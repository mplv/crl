#include <stdlib.h>
#include <string.h>
#include "debug.h"
#include "arraylist/arraylist.h"

void RL_CreateDebug()
{
    // db = calloc(1, sizeof(RL_Debug));
    // db->l = AL_New();
}

void RL_AddDebugMessage(char *c)
{
    // RL_DebugMessage *dm = calloc(1, sizeof(RL_DebugMessage));
    // strncpy(dm->debug_message, c, 80);
    // AL_Add(db->l,dm);
}

RL_DebugMessage *RL_GetDebugMessage(int pos)
{
    // return AL_Get(db->l, pos);
	return NULL;
}

RL_DebugMessage *RL_GetLastDebugMessage()
{
    // return AL_Get(db->l, AL_Size(db->l) - 1);
	return NULL;
}

void RL_DestroyDebug()
{
    // while (AL_Size(db->l) > 0) {
    //     RL_DebugMessage *dm = AL_RemoveLast(db->l);
    //     free(dm);
    // }
    // AL_Destroy(db->l);
    // free(db);
}
