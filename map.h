#ifndef map_h
#define map_h

#include "runtimecontext.h"

void RL_GenerateMap(RL_RTContext *rtc);
void RL_CreateMap(RL_RTContext* rl, int w, int h);
void RL_DestroyMap(RL_RTContext* rl, int w);
int RL_ValidMove(RL_RTContext *rtc, RL_Entity *ent, int dir);

#endif /* map_h */
