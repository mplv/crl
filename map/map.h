#ifndef map_h
#define map_h

#include "textures/textures.h"
#include "entity/entity.h"
#include "generator/generator.h"
#include "arraylist/arraylist.h"

typedef struct _RL_MAP {
	// all of the textures in a map should use new data structure
	texture_id_type **map;
	// valid move for an entity at an (x,y) in a direction
	int (*ValidMove) (struct _RL_MAP *,RL_Entity*,int dir);
	// gets the color of the tile
	void (*GetColor) (struct _RL_MAP*, unsigned char arr[3], texture_id_type);
	// size of map in tiles
	int sizeX;
	int sizeY;
	// the tiles that entities cannot move through
	texture_id_type *obstacles;
	// len of the obstacles array
	int obstaclesLen;
	// climate of the map
	// useful for blending/changing the map climates
	int climate;

	ArrayList* creatures;

	// used to allow a snapshot of the generator before
	// map creation
	RL_Generator initalGeneratorState;

	void (*DestroyMap)(struct _RL_MAP *, int);
} RL_Map;

// Def for a map generator.
// each generator has to have a create function
typedef struct _RL_MAP_GENERATOR {
	RL_Map* (*Create)(int,int,RL_Generator*);
} RL_MapGenerator;

// generator holder for all generators
typedef struct _RL_MAP_GENERATOR_HOLDER {
	ArrayList* list;
	void (*Register)(struct _RL_MAP_GENERATOR_HOLDER*,RL_MapGenerator*);
	RL_Map* (*Generate) (struct _RL_MAP_GENERATOR_HOLDER* mgh, int w, int h, RL_Generator* gen);
} RL_MapGeneratorHolder;

// functions global to all generators
int RL_ValidMove(RL_Map *m, RL_Entity *ent, int dir);
void RL_DestroyMap(RL_Map *m, int w);
void RL_MapSave(RL_Map *map, const char *base_path);
RL_Map* RL_MapLoad(RL_MapGeneratorHolder *mgh, const char *base_path);
RL_Map* RL_MapLoadGenerate(RL_MapGeneratorHolder* mgh, RL_Generator* gen, int climate, int w, int h);
void RL_PopulateMap(RL_Map* m, ArrayList *masterList);

// map generator holder functions
RL_MapGeneratorHolder* RL_CreateMapGeneratorHolder();
void RL_DestroyMapGeneratorHolder(RL_MapGeneratorHolder *mgh);


#endif /* map_h */
