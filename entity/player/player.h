#ifndef player_h
#define player_h


#include "arraylist/arraylist.h"
#include "entity/entity.h"

typedef struct {
    RL_Entity *ent;
    ArrayList *inventory;
} RL_Player;

RL_Player* RL_NewPlayer();
void RL_DestroyPlayer(RL_Player *rtc);

#endif /* player_h */
