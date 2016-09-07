#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "arraylist/arraylist.h"
#include "generator/generator.h"
#include "map.h"
#include "textures/textures.h"
#include "runtimecontext/runtimecontext.h"

// register a generator with the holder
void RL_MapGeneratorHolderRegister(RL_MapGeneratorHolder* mgh, RL_MapGenerator* mg) {
	AL_Add(mgh->list, mg);
}

// use the holder to generate a map
// TODO make it blend maps so that it makes some sense
RL_Map *RL_GenerateMap(RL_MapGeneratorHolder* mgh, int w, int h, RL_Generator* gen)
{
	RL_MapGenerator* mg = AL_Get(mgh->list,1);
	return mg->Create(w,h,gen);
}

// create the map generator holder container
RL_MapGeneratorHolder* RL_CreateMapGeneratorHolder()
{
	RL_MapGeneratorHolder *mgh = calloc(1,sizeof(RL_MapGeneratorHolder));
	mgh->list = AL_New();
	mgh->Register = &RL_MapGeneratorHolderRegister;
	mgh->Generate = &RL_GenerateMap;
	return mgh;
}

void RL_DestroyMapGeneratorHolder(RL_MapGeneratorHolder *mgh)
{
	// force programmer to remove all of the elements in list
	// to prevent them from forgetting that they need to deal with the pointers
	// stored in there
	while(AL_Size(mgh->list) > 0){AL_RemoveLast(mgh->list);}
	AL_Destroy(mgh->list);
	free(mgh);
}

// free the map pointer that we allocated
void RL_DestroyMap(RL_Map *m, int w)
{
    int i = 0;
    for (; i < w; i++) {
        free(m->map[i]);
    }
    free(m->map);
	free(m->obstacles);
	free(m);
}

// Check for a valid move based on a map, entity and a directon of movement
int RL_ValidMove(RL_Map *m, RL_Entity *ent, int dir)
{
    int b = 1;
    int i = 0;
    int x = ent->x;
    int y = ent->y;
    switch (dir) {
        case 0: // w
            y -= 1;
            break;
        case 1: // s
            y += 1;
            break;
        case 2: // a
            x -= 1;
            break;
        case 3: // d
            x += 1;
            break;
    }

	// make sure that x and y are within bounds
    if (x < 0) x = 0;
    if (y < 0) y = 0;

    if (x > (m->sizeX - 1))
    {
        x = m->sizeX - 1;
    }

    if (y > (m->sizeY - 1))
    {
        y = m->sizeY - 1;
    }

	// make sure that we are not moving through walls and such
    for (i = 0; i < m->obstaclesLen; i++) {
        if (m->map[x][y] == m->obstacles[i])
            b = 0;
    }
    return b;
}
