#ifndef RL_CREATURE
#define RL_CREATURE

#include "config.h"
#include "arraylist.h"

typedef struct {
    // max name is 20 chars long
    char name[20];
    // vers of game that can gen this enemy
    int pre;
    // stats
    int atk;
    int def;
    int evn;
    int lck;
    // which table this enemy drops items from
    int droptable;
    // which climates this mob spawns in
    char climate;
    // color of the text for the sprite
    int r,g,b;
} RL_Creature;

void RL_FreeCreaturesList(ArrayList *l);
ArrayList* RL_LoadAllCreatures(RL_Config *c);
void RL_PrintCreature(RL_Creature *c);
#endif
