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
void RL_PlayerSave(RL_Player *player, const char *);
RL_Player* RL_PlayerLoad(const char *);

#endif /* player_h */
