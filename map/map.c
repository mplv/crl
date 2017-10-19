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

// add a map creation function to the list of available functions
// in order as defined in map.h
void MapGeneratorAdd(MapGenerator* mg, Map* (*MAPCreate)(int, int, Random *)) {
	ListAdd(mg->list, MAPCreate);
}

// use the holder to generate a map
// TODO make it blend maps so that it makes some sense
Map *GenerateMap(MapGenerator* mg, int w, int h, Random* gen)
{
	int mapType = gen->GenerateNumber(gen) % ListSize(mg->list);
	Map* (*func)(int, int, Random *) = ListGet(mg->list,mapType);
	if (func != NULL) {
		return func(w,h,gen);
	}
	return NULL;
}

// create the map generator holder container
MapGenerator* CreateMapGenerator()
{
	MapGenerator *mg = calloc(1,sizeof(MapGenerator));
	mg->list = NewList();
	return mg;
}

void DestroyMapGenerator(MapGenerator *mg)
{
	// force programmer to remove all of the elements in list
	// to prevent them from forgetting that they need to deal with the pointers
	// stored in there
	while(ListSize(mg->list) > 0){ListRemoveLast(mg->list);}
	ListDestroy(mg->list);
	free(mg);
}

void MapSave(Map *map, const char *base_path) {
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
	DebugMessageStart(LOG);
	printf("%s", save_path);
    DebugMessageEnd();
	FILE *f = fopen(save_path, "w");
	if (f) {
		for (i = 0; i < 6; i++) {
			fprintf(f, "%lu\n", map->initalGeneratorState.state[i]);
		}
		fprintf(f, "%d\n", map->climate);
		fprintf(f, "%d\n", map->sizeX);
		fprintf(f, "%d\n", map->sizeY);
		fclose(f);
		CreatureListSave(map->creatures,base_path);
	}
	free(save_path);
}

Map* MapLoadGenerate(MapGenerator* mg, Random* gen, climates_t climate, int w, int h)
{
	Map* (*func)(int, int, Random *) = ListGet(mg->list,climate);
	if (func != NULL) {
		return func(w,h,gen);
	}
	return NULL;
}

Map* MapLoad(MapGenerator *mg, const char *base_path) {
	Map *map = NULL;
	unsigned long arr[6];
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
	DebugMessageStart(LOG);
	printf("%s", load_path);
    DebugMessageEnd();
	FILE *f = fopen(load_path, "r");
	if (f) {
		for (i = 0; i < 6; i++) {
			fscanf(f, "%lu\n", &(arr[i]));
		}
		fscanf(f, "%d\n", &climate);
		fscanf(f, "%d\n", &sizeX);
		fscanf(f, "%d\n", &sizeY);
		fclose(f);
		// TODO do not forget to free this!!!!
		Random* gen = NewRandomWithState(arr);
		map = MapLoadGenerate(mg, gen, climate, sizeX, sizeY);
		DestroyRandom(gen);
	}

	free(load_path);
	return map;
}

static int openTile(Map* m, int x, int y)
{
	int i = 0;
	for (i = 0; i < m->obstaclesLen; i++)
	{
		if (m->obstacles[i] == m->map[x][y])
		{
			return 0;
		}
	}
	return 1;
}

void PlayerSpawn(Map *m, Player* p, Random* gen, ArrayList* creatures)
{
    int foundPosition = 0;
    int x,y,i;
    while (!foundPosition) {
        x = gen->GenerateNumber(gen) % m->sizeX;
        y = gen->GenerateNumber(gen) % m->sizeY;
		for (i = 0; i < ListSize(creatures); i++)
		{
			Creature *c = ListGet(creatures,i);
			if (x == c->ent.x && y == c->ent.y)
			{
				foundPosition = 0;
				continue;
			}
		}
        if (openTile(m,x,y))
        {
            p->ent->x = x;
            p->ent->y = y;
            foundPosition = 1;
        }
    }
}

void PopulateMap(Map* m, Random* gen, ArrayList* masterList)
{
	int numberOfCreatures = 10;
	while(numberOfCreatures > 0)
	{
		// TODO make better way to add creatures to a map
		// use the climate to determine what gets added
		// and then find some way to add a good distribution of them
		// also need to copy the creatuers over from the master list
		// as otherwise you modify the the creature for the entire game
		Creature* creature = calloc(1, sizeof(Creature));
		memcpy(creature, ListGet(masterList,0), sizeof(Creature));
		creature->nameAllocd = 0;
		int foundPosition = 0;
		int x,y;
		while (!foundPosition) {
			x = gen->GenerateNumber(gen) % m->sizeX;
			y = gen->GenerateNumber(gen) % m->sizeY;
			if (openTile(m,x,y))
			{
				creature->ent.x = x;
				creature->ent.y = y;
				foundPosition = 1;
			}
		}

		ListAdd(m->creatures, creature);
		numberOfCreatures--;
	}
}

// free the map pointer that we allocated
void DestroyMap(Map *m, int w)
{
    int i = 0;
    for (; i < w; i++) {
        free(m->map[i]);
    }
    free(m->map);
	free(m->obstacles);
	printf("creature list size: %d\n", ListSize(m->creatures));
	Creature* c = ListRemoveLast(m->creatures);
	while (c != NULL)
	{
		if (c->nameAllocd == 1)
		{
			free(c->name);
		}
		free(c);
		c = ListRemoveLast(m->creatures);
	}
	DebugMessageStart(LOG);
	printf("creature list size: %d\nCalling ListDestroy on creaturelist", ListSize(m->creatures));
	DebugMessageEnd();
	// free the list
	// note only works if the list size is 0
	ListDestroy(m->creatures);
	free(m);
}

// Check for a valid move based on a map, entity and a directon of movement
int ValidMove(Map *m, Player* p, Entity *ent, int dir)
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
    for (i = 0; i < m->obstaclesLen; i++)
	{
        if (m->map[x][y] == m->obstacles[i])
            b = 0;
    }
	for (i=0; i< ListSize(m->creatures); i++)
	{
		Creature *c = ListGet(m->creatures, i);
		if (c->ent.x == x && c->ent.y == y)
			b = 0;
	}
	if (p->ent->x == x && p->ent->y == y)
		b = 0;
    return b;
}

// color the map
void MapGetColor(Map *m, unsigned char rgb[3], texture_id_type id)
{
	switch (m->climate) {

		case CAVE:
			CaveGetColors(m,rgb,id);
		break;
		case FOREST:
			ForestGetColors(m,rgb,id);
		break;
		default:
		rgb[0] = 255;
		rgb[1] = 255;
		rgb[2] = 255;
		break;
	}
}
