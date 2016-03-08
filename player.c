#include <stdlib.h>
#include "player.h"
#include "item.h"
#include "entity.h"
#include "arraylist.h"

RL_Player* RL_NewPlayer()
{
    RL_Player *player = calloc(1, sizeof(RL_Player));
    player->ent = RL_NewEntity();
    player->inventory = AL_New();
    return player;
}

void RL_DestroyPlayer(RL_Player *player)
{
    while (AL_Size(player->inventory) > 0) {
        RL_Item *i = AL_RemoveLast(player->inventory);
        free(i);
    }
    AL_Destroy(player->inventory);
    free(player->ent);
    free(player);
}
