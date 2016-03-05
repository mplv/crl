#ifndef player_h
#define player_h


#include "arraylist.h"

typedef struct {
    int x,y;
    int hp;
    int maxHp;
    int mp;
    int maxMp;
    int atk;
    int def;
    int evn;
    int lck;
    int mst;
//    ArrayList *inventory;
}RL_Player;

RL_Player* RL_NewPlayer();
void RL_DestroyPlayer(RL_Player *rtc);

#endif /* player_h */
