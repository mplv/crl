#ifndef entity_h
#define entity_h

#include <stdio.h>
#include "textures/textures.h"

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
	texture_id_type texture;
	// color of the text for the sprite
    unsigned int r,g,b;
}RL_Entity;

RL_Entity* RL_NewEntity();
void RL_DestroyEntity(RL_Entity *rtc);
void RL_EntityMove(RL_Entity *ent, int dir, int maxX, int maxY);
void RL_EntitySave(FILE* f, RL_Entity* e);
void RL_EntityLoad(FILE* f, RL_Entity* e);

#endif /* entity_h */
