#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "generator/generator.h"
#include "textures/textures.h"
#include "runtimecontext/runtimecontext.h"
#include "config/config.h"
#include "entity/creature/creature.h"
#include "map/map.h"
#include "map/generators.h"
#include "arraylist/arraylist.h"
#include "entity/player/player.h"
#include "debug/debug.h"
#include "gameinputstate/gameinputstate.h"
#include "draw/draw.h"

// TODO data structure for data of game so that refs are just numbers not pointers
//		perhaps static arrays for base items and stuff.  Random items generated
//		and use item id of base items?
//		Perhaps we just use the arraylist and hope everything ends up in the right
//		place?
//		2017-01-10:
//			Should use arraylist not pointers for data so that way it is eaiser to use
//			and keep track of memory
// TODO map generators - need more but some are done like caves
//		generators:
//			water pools
//			standard room based dungeon
//			wilderness
//			mountain? - hard to do because of Z level
//			plains - flat land or rolling
// TODO time system.  real or soft?
//			real time - would be more interactive as things move around
//				and allow for interesting ai
//			soft time - would allow more time to plan out paths and decide
//				moves.  Also allows less cpu usage over time on low end machines
// TODO monster spawning and stats
//			have list of all monster defs
//			need good way to spawn them
//			also need stats and balancing
// TODO player stats and displays
//			how to do displays for health?
//				little bar above char or seperate panel?
//				maybe another window? - window with stats?
//				put stats in title bar?
// TODO monster targeting besides moving into monster
//			targeting systems like
//				two key - press to activate target systems then press key to act on target
//				scroll to target - move cursor to target then select to act?
//				both because of aoe skills?
//				use mouse to target? - we have a window might as well use it
// TODO Saving
//			writing the important bits to a save file
//				in this case i am going to use a save folder
//				it is easier and also allows me to verify that everything is working
//				rather than use one single file and shove everything together
//				does allow an eaiser time to cheat and stuff but oh well

int main (int argc, const char *argv[])
{
	if (getuid()==0){
		printf("Running this program as root is unsafe.\nDo not run this program as root!\n");
		exit(EXIT_FAILURE);
	}

	int err = 0;
	int flags = 0;
	int saveNotLoaded = 1;

	// choosing a res that give a nice size for the tiles
	// int windowSizeX = 1024;
	// int windowSizeY = 896;
	// sdl setup
  	err = SDL_Init(SDL_INIT_VIDEO);
  	if (err < 0) {
	  fprintf(stderr,
				  "\nUnable to initialize SDL:  %s\n",
				  SDL_GetError()
				 );
  		exit(1);
  	}

	// init the image library
  	flags = IMG_Init(IMG_INIT_PNG);
  	if ((flags & IMG_INIT_PNG) != IMG_INIT_PNG) {
  		printf("Using SDL_Image failed.  Please make sure that SDL_Image is installed\n");
  		exit(2);
  	}

	// get current working directory. In other words get where the exe is
	char *base_path = SDL_GetBasePath();

	// PUT conf load here for window size
	// load the config file
	//TODO use the base path
	RL_Config* conf = RL_LoadConfig(base_path,"data/conf.txt");
	if (conf == NULL) {
		printf("Config not loaded quitting...\n");
		IMG_Quit();
		SDL_Quit();
		return -1;
	}

	// https://wiki.libsdl.org/SDL_SetWindowIcon
  	// windowSizeX and Y should be based on tile width and height
  	SDL_Window *window = SDL_CreateWindow("RL", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
  		conf->width, conf->height, SDL_WINDOW_SHOWN);
  	if (window == NULL) {
  		printf("Error creating window\n");
		IMG_Quit();
		SDL_Quit();
  		exit(3);
  	}

  	//renderer, err := sdl.CreateRenderer(window,-1,sdl.RENDERER_ACCELERATED|sdl.RENDERER_PRESENTVSYNC)
  	SDL_Renderer *renderer = SDL_CreateRenderer(window,-1,0);
  	if (renderer == NULL) {
  		printf("Error creating renderer to use with window\n");
		SDL_DestroyWindow(window);
		IMG_Quit();
		SDL_Quit();
  		exit(4);
  	}

	// create the structure for the textures
	err = SDL_Init(SDL_INIT_VIDEO);
    Textures *textures = Textures_New();
	if (textures == NULL) {
		SDL_free(base_path);
		printf("Could not create textures\n");
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		IMG_Quit();
		SDL_Quit();
		exit(5);
	}

	// after creating space for the textures load them
	int loaded = Textures_Load(textures, base_path, renderer);
	if (loaded <= 0 && loaded != NUM_TEXTURES) {
		SDL_free(base_path);
		Textures_Destroy(textures);
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		IMG_Quit();
		SDL_Quit();
		printf("Textures not loaded\n");
		return -1;
	}

	// create a runtime context
	RL_RTContext *rtc = RL_CreateRTContext();
	rtc->conf = conf;
	// set the textures
	rtc->textures = textures;

	// create a map
	rtc->mapGenHolder = RL_CreateMapGenerator();
	if (!rtc->mapGenHolder)
	{
		SDL_free(base_path);
		RL_FreeGenerator(rtc->gen);
		Textures_Destroy(textures);
		RL_FreeConfig(rtc->conf);
		RL_FreeRTContext(rtc);
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		IMG_Quit();
		SDL_Quit();
		printf("Map generator could not be allocated quitting...\n");
		return -1;
	}

	RL_MapGeneratorAdd(rtc->mapGenHolder,&RL_CreateForestMap);
	RL_MapGeneratorAdd(rtc->mapGenHolder,&RL_CreateCaveMap);

	// if this is the first time the game is being run
	// set the generator seed
	// also do not try to load the old save file
	if (rtc->conf->gen == -1)
	{
		rtc->conf->gen = time(NULL);
		rtc->gen = RL_GeneratorNew();
	}
	else
	{
		RL_LoadSave(rtc);
		saveNotLoaded = 0;
	}


	// load the creatures but dont do anything with them just yet
	// TODO this requires dirent not sure if it can be used on windows
	// might just be better to open a file with a list of creatures and
	// loop through that to get the data files
	rtc->creatures = RL_LoadAllCreatures(rtc->conf);

	if (saveNotLoaded) {
		// generate a map
		rtc->map = RL_GenerateMap(rtc->mapGenHolder, rtc->conf->mapsizex, rtc->conf->mapsizey, rtc->gen);
		// create a player
		rtc->player = RL_NewPlayer();
		// set player color
		// TODO get values from config?
		rtc->player->ent->r = 255;
		rtc->player->ent->g = 10;
		rtc->player->ent->b = 50;
		RL_PopulateMap(rtc->map, rtc->creatures);
	}




	// create a game input state that allows for different inputs
	// for different screens
	// TODO add the other screens
	GameInputStateHolder *gameInputStateHolder = GIS_NewHolder();
	GameInputState *main_screen = GIS_Main_State_New();
	gameInputStateHolder->Register(gameInputStateHolder, main_screen);


	// inital draw
	SDL_SetRenderDrawColor(renderer,0,0,0,255);
	SDL_RenderClear(renderer);
	RL_Draw(rtc,renderer);
	SDL_RenderPresent(renderer);

	// event loop
	volatile int running = 1;
	int have_event = 0;
	SDL_Event event;
	while (running) {

		have_event = SDL_WaitEvent(&event);
		if (have_event) {
			switch (event.type) {
				// check key presses
				// global key presses are in here
				// otherwise the game input state gets called
				case SDL_KEYDOWN:
					if (event.key.keysym.sym == SDLK_q && ((event.key.keysym.mod & KMOD_CTRL) > 0))
					{
						running = 0;
						break;
					}
					// do stuff
					gameInputStateHolder->Do(gameInputStateHolder,rtc,event.key.keysym.sym);
					// move the creatures around?
					break;
				case SDL_QUIT:
					running = 0;
					break;
			}
		}
		// draw here so that when running in "real time" it will update the screen
		SDL_SetRenderDrawColor(renderer,0,0,0,255);
		SDL_RenderClear(renderer);
		RL_Draw(rtc,renderer);
		SDL_RenderPresent(renderer);
	}
	// TODO save here
	RL_Save(rtc);

	// start clean up
	SDL_free(base_path);
	GIS_FreeHolder(gameInputStateHolder);
	Textures_Destroy(textures);
	RL_FreeCreaturesList(rtc->creatures);
	RL_DestroyPlayer(rtc->player);
	RL_DestroyMap(rtc->map, rtc->conf->mapsizex);
	RL_DestroyMapGenerator(rtc->mapGenHolder);
	RL_FreeGenerator(rtc->gen);
	RL_FreeConfig(rtc->conf);
	RL_FreeRTContext(rtc);

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	IMG_Quit();
	SDL_Quit();
	return 0;
}
