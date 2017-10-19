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
    int r,g,b;
}Entity;

Entity* NewEntity();
void DestroyEntity(Entity *ent);
void EntityMove(Entity *ent, int dir, int maxX, int maxY);
void EntitySave(FILE* f, Entity* e);
void EntityLoad(FILE* f, Entity* e);

#endif /* entity_h */
