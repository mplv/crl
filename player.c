#include <stdlib.h>
#include "player.h"
#include "arraylist.h"

RL_Player* RL_NewPlayer()
{
    RL_Player *player = calloc(1, sizeof(RL_Player));
//    player->inventory = AL_New();
    player->hp = 50;
    player->maxHp = 90;
    player->mp = 83;
    player->maxMp = 137;
    return player;
}

void RL_DestroyPlayer(RL_Player *player)
{
//    while (AL_Size(player->inventory) > 0) {
//        Item *i = AL_RemoveLast(player->inventory);
//        free(i);
//    }
//    AL_Destroy(player->inventory);
    free(player);
}
