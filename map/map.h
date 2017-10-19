#ifndef map_h
#define map_h

#include "textures/textures.h"
#include "entity/entity.h"
#include "entity/player/player.h"
#include "generator/generator.h"
#include "arraylist/arraylist.h"

typedef enum {
	FOREST,
	CAVE,
	NUM_CLIMATES
} climates_t;

typedef struct _MAP {
	// all of the textures in a map should use new data structure
	texture_id_type **map;

	// size of map in tiles
	int sizeX;
	int sizeY;

	// the tiles that entities cannot move through
	texture_id_type *obstacles;

	// len of the obstacles array
	int obstaclesLen;

	// climate of the map
	// useful for blending/changing the map climates
	// or determining creature spawn or map type
	climates_t climate;

	// each creature in this is so that we do not change the templates
	// in rtc->creaturess
	ArrayList* creatures;

	// used to allow a snapshot of the generator before
	// map creation so that when we ask for generation
	// it gives us the same map we had when we stopped playing
	Random initalGeneratorState;
} Map;

// generator holder for all generators
typedef struct _MAP_GENERATOR {
	ArrayList* list;
} MapGenerator;

// functions global to all generators
int ValidMove(Map *m, Player* p, Entity *ent, int dir);
void DestroyMap(Map *m, int w);
void MapSave(Map *map, const char *base_path);
Map* MapLoad(MapGenerator *mg, const char *base_path);
Map* MapLoadGenerate(MapGenerator* mg, Random* gen, climates_t climate, int w, int h);
void PopulateMap(Map* m, Random *gen, ArrayList *masterList);
void MapGetColor(Map *m, unsigned char rgb[3], texture_id_type id);
void PlayerSpawn(Map *m, Player* p, Random* gen, ArrayList* creatures);

// map generator holder functions
MapGenerator* CreateMapGenerator();
void DestroyMapGenerator(MapGenerator *mg);
void MapGeneratorAdd(MapGenerator* mg, Map* (*MAPCreate)(int, int, Random *));
Map *GenerateMap(MapGenerator* mg, int w, int h, Random* gen);

// map coloring functions
void ForestGetColors(Map *m, unsigned char rgb[3], texture_id_type id);
void CaveGetColors(Map *m, unsigned char rgb[3], texture_id_type id);


#endif /* map_h */
