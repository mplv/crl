#include <stdlib.h>
#include <stdio.h>
#include "entity.h"
#include "debug/debug.h"

Entity* NewEntity()
{
    Entity *ent = calloc(1, sizeof(Entity));
    ent->hp = 50;
    ent->maxHp = 90;
    ent->mp = 83;
    ent->maxMp = 137;
    return ent;
}

void DestroyEntity(Entity *ent)
{
    free(ent);
}

void EntitySave(FILE* f, Entity* e)
{
	// write the entity to the given file
	fprintf(f, "%d\n", e->x);
	fprintf(f, "%d\n", e->y);
	fprintf(f, "%d\n", e->hp);
	fprintf(f, "%d\n", e->maxHp);
	fprintf(f, "%d\n", e->mp);
	fprintf(f, "%d\n", e->maxMp);
	fprintf(f, "%d\n", e->atk);
	fprintf(f, "%d\n", e->def);
	fprintf(f, "%d\n", e->evn);
	fprintf(f, "%d\n", e->lck);
	fprintf(f, "%d\n", e->mst);
	fprintf(f, "%d\n", e->texture);
	fprintf(f, "%d\n", e->r);
	fprintf(f, "%d\n", e->g);
	fprintf(f, "%d\n", e->b);
}

void EntityLoad(FILE* f, Entity* e)
{
	int tex = 0;
	// read the entity to the given file
	fscanf(f, "%d\n", &e->x);
	fscanf(f, "%d\n", &e->y);
	fscanf(f, "%d\n", &e->hp);
	fscanf(f, "%d\n", &e->maxHp);
	fscanf(f, "%d\n", &e->mp);
	fscanf(f, "%d\n", &e->maxMp);
	fscanf(f, "%d\n", &e->atk);
	fscanf(f, "%d\n", &e->def);
	fscanf(f, "%d\n", &e->evn);
	fscanf(f, "%d\n", &e->lck);
	fscanf(f, "%d\n", &e->mst);
	fscanf(f, "%d\n", &tex);
	fscanf(f, "%d\n", &e->r);
	fscanf(f, "%d\n", &e->g);
	fscanf(f, "%d\n", &e->b);
	e->texture = tex;
}

void EntityMove(Entity *ent, int dir, int maxX, int maxY)
{
    switch (dir) {
        case 0: // w
            ent->y -= 1;
            break;
        case 1: // s
            ent->y += 1;
            break;
        case 2: // a
            ent->x -= 1;
            break;
        case 3: // d
            ent->x += 1;
            break;
    }
    ent->x = ent->x < 0 ? 0 : ent->x;
    ent->y = ent->y < 0 ? 0 : ent->y;

    if (ent->x > maxX)
    {
        ent->x = maxX;
    }

    if (ent->y > maxY)
    {
        ent->y = maxY;
    }
}
