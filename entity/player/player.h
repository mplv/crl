#ifndef player_h
#define player_h


#include "arraylist/arraylist.h"
#include "entity/entity.h"

typedef struct {
    Entity *ent;
    ArrayList *inventory;
} Player;

Player* NewPlayer();
void DestroyPlayer(Player *player);
void PlayerSave(Player *player, const char *);
Player* PlayerLoad(const char *);

#endif /* player_h */
