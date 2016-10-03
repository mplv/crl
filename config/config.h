#ifndef RL_CONFIG
#define RL_CONFIG

// NOTE that this is read only once in the event loop
// This structure will be passed to nearly all of the games
// functions! Do not miss use
typedef struct _RL_Config {
	// this makes the structure read only if set
	char readonly;
	char* path;
	const char* base_path;
	// include key bindings here?
	//
	// in tiles or pixels?
	int width;
	int height;
	// pixels for tile height
	int tileWidth;
	int tileHeight;
	int widthToTiles;
	int heightToTiles;
	int gen;
	int mapsizex;
	int mapsizey;

} RL_Config;

RL_Config* RL_LoadConfig(const char *base_path,const char *configFilePath);
void RL_FreeConfig(RL_Config *config);
#endif
