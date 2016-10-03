#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "debug/debug.h"
#include "arraylist/arraylist.h"
#include "generator/generator.h"
#include "map.h"
#include "entity/creature/creature.h"
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

void RL_MapSave(RL_Map *map, const char *base_path) {
	const char *save_loc = "data/save/map.txt";
	int basePathLen = 0;
	int saveLocLen = 0;
	int i = 0;
	basePathLen = strnlen(base_path,256);
	saveLocLen = strnlen(save_loc,256);
	char *save_path = calloc((basePathLen + saveLocLen + 1), sizeof(char));
	for (i = 0; i < basePathLen; i++) {
		save_path[i] = base_path[i];
	}
	for (i = 0; i < saveLocLen; i++) {
		save_path[i + basePathLen] = save_loc[i];
	}
	save_path[basePathLen+saveLocLen] = '\0';
	RL_DebugMessage(LOG, save_path);
	FILE *f = fopen(save_path, "w");
	if (f) {
		for (i = 0; i < 4096; i++) {
			#ifdef __L64__
			fprintf(f, "%lu\n", map->initalGeneratorState.Q[i]);
			#else
			fprintf(f, "%u\n", map->initalGeneratorState.Q[i]);
			#endif
		}
		#ifdef __L64__
        fprintf(f, "%lu\n", map->initalGeneratorState.c);
        #else
        fprintf(f, "%u\n", map->initalGeneratorState.c);
        #endif
		fprintf(f, "%d\n", map->climate);
		fprintf(f, "%d\n", map->sizeX);
		fprintf(f, "%d\n", map->sizeY);
		fclose(f);
		RL_CreatureListSave(map->creatures,base_path);
	}
	free(save_path);
}

RL_Map* RL_MapLoadGenerate(RL_MapGeneratorHolder* mgh, RL_Generator* gen, int climate, int w, int h)
{
	RL_MapGenerator* mg = AL_Get(mgh->list,climate);
	return mg->Create(w,h,gen);
}

RL_Map* RL_MapLoad(RL_MapGeneratorHolder *mgh, const char *base_path) {
	RL_Map *map = NULL;
	RL_Generator gen;
	int climate;
	int sizeX;
	int sizeY;
	const char *load_loc = "data/save/map.txt";
	int basePathLen = 0;
	int loadLocLen = 0;
	int i = 0;
	basePathLen = strnlen(base_path,256);
	loadLocLen = strnlen(load_loc,256);
	char *load_path = calloc((basePathLen + loadLocLen + 1), sizeof(char));
	for (i = 0; i < basePathLen; i++) {
		load_path[i] = base_path[i];
	}
	for (i = 0; i < loadLocLen; i++) {
		load_path[i + basePathLen] = load_loc[i];
	}
	load_path[basePathLen+loadLocLen] = '\0';
	RL_DebugMessage(LOG, load_path);
	FILE *f = fopen(load_path, "r");
	if (f) {
		for (i = 0; i < 4096; i++) {
			#ifdef __L64__
			fscanf(f, "%lu\n", &(gen.Q[i]));
			#else
			fscanf(f, "%u\n", &(gen.Q[i]));
			#endif
		}
		#ifdef __L64__
		fscanf(f, "%lu\n", &(gen.c));
		#else
		fscanf(f, "%u\n", &(gen.c));
		#endif
		fscanf(f, "%d\n", &climate);
		fscanf(f, "%d\n", &sizeX);
		fscanf(f, "%d\n", &sizeY);
		fclose(f);
		gen.GenerateNumber = &CMWC4096;
		map = RL_MapLoadGenerate(mgh, &gen,climate, sizeX,sizeY);
	}

	free(load_path);
	return map;
}

void RL_PopulateMap(RL_Map* m, ArrayList* masterList)
{
	int numberOfCreatures = 10;
	while(numberOfCreatures > 0)
	{
		// TODO make better way to add creatures to a mpa
		// use the climate to determine what gets added
		// and then find some way to add a good distribution of them
		// also need to copy the creatuers over from the master list
		// as otherwise you modify the the creature for the entire game
		RL_Creature* creature = calloc(1, sizeof(RL_Creature));
		memcpy(creature, AL_Get(masterList,0), sizeof(RL_Creature));
		creature->ent.x = numberOfCreatures;
		creature->ent.y = numberOfCreatures;
		AL_Add(m->creatures, creature);
		numberOfCreatures--;
	}
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
	RL_Creature* c = AL_RemoveLast(m->creatures);
	while (c != NULL)
	{
		free(c);
		c = AL_RemoveLast(m->creatures);
	}
	// free the list
	// note only works if the list size is 0
	AL_Destroy(m->creatures);
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
