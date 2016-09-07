#ifndef debug_h
#define debug_h

#include "arraylist/arraylist.h"

typedef struct {
    ArrayList *l;
}RL_Debug;

typedef struct {
    char debug_message[80];
}RL_DebugMessage;


RL_DebugMessage *RL_GetLastDebugMessage();
RL_DebugMessage *RL_GetDebugMessage(int pos);
void RL_AddDebugMessage(char *c);
void RL_CreateDebug();
void RL_DestroyDebug();

#endif /* debug_h */
