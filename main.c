#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
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
// TODO map generators - need more but some are done like caves
//		generators:
//			water pools
//			standard room based dungeon
//			wilderness
//			mountain? - hard to do because of Z level
//			plains
// TODO time system.  real or soft?
//			real time - would be more interactive as things move around
//				and allow for interesting ai
//			soft time - would allow more time to plan out paths and decide
//				moves.  Also allows less cpu usage.
// TODO monster spawning and stats
//			have list of all monster defs
//			need good way to spawn them
//			also need stats and balancing
// TODO player stats and displays
//			how to do displays for health?
//				little bar above char or seperate panel?
//				maybe another window?
// TODO monster targeting besides moving into monster
//			targeting systems like
//				two key - press to activate target systems then press key to act on target
//				scroll to target - move cursor to target then select to act?

int main (int argc, const char *argv[])
{
	int err = 0;
	int flags = 0;
	// choosing a res that give a nice size for the tiles
	int windowSizeX = 1664;
	int windowSizeY = 800;
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

	// https://wiki.libsdl.org/SDL_SetWindowIcon
  	// windowSizeX and Y should be based on tile width and height
  	SDL_Window *window = SDL_CreateWindow("RL", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
  		windowSizeX, windowSizeY, SDL_WINDOW_SHOWN);
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
	// get current working directory. In other words get where the exe is
	char *base_path = SDL_GetBasePath();

	// create the structure for the textures
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
	// set the textures
	rtc->textures = textures;
	// load the config file
	//TODO use the base path
	rtc->conf = RL_LoadConfig(base_path,"data/conf.txt");
	if (rtc->conf == NULL) {
		SDL_free(base_path);
		Textures_Destroy(textures);
		RL_FreeRTContext(rtc);
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		IMG_Quit();
		SDL_Quit();
		printf("Config not loaded quitting...\n");
		return -1;
	}

	// if this is the first time the game is being run
	// set the generator seed
	if (rtc->conf->gen == -1)
	{
		rtc->conf->gen = time(NULL);
	}

	// initalize the generator
	rtc->gen = RL_GeneratorNew(rtc->conf->gen);
	if (rtc->conf == NULL) {
		SDL_free(base_path);
		Textures_Destroy(textures);
		RL_FreeConfig(rtc->conf);
		RL_FreeRTContext(rtc);
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		IMG_Quit();
		SDL_Quit();
		printf("Config not loaded quitting...\n");
		return -1;
	}

	// create a map
	rtc->mapGenHolder = RL_CreateMapGeneratorHolder();
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
		printf("Map could not be allocated quitting...\n");
		return -1;
	}

	RL_MapGenerator forest;
	RL_MapGenerator cave;
	forest.Create = &RL_CreateForestMap;
	cave.Create = &RL_CreateCaveMap;

	rtc->mapGenHolder->Register(rtc->mapGenHolder,&forest);
	rtc->mapGenHolder->Register(rtc->mapGenHolder,&cave);

	// generate a map
	rtc->map = rtc->mapGenHolder->Generate(rtc->mapGenHolder, rtc->conf->mapsizex, rtc->conf->mapsizey, rtc->gen);

	// load the creatures but dont do anything with them just yet
	// TODO this requires dirent not sure if it can be used on windows
	rtc->creatures = RL_LoadAllCreatures(rtc->conf);
	// create a player
	rtc->player = RL_NewPlayer();
	// set player color
	// TODO get values from config?
	rtc->player->ent->r = 0;
	rtc->player->ent->g = 255;
	rtc->player->ent->b = 255;

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
					if (event.key.keysym.sym == SDLK_q && event.key.keysym.mod == KMOD_LCTRL)
					{
						running = 0;
						break;
					}
					// do stuff
					gameInputStateHolder->Do(gameInputStateHolder,rtc,event.key.keysym.sym);
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


	// start clean up
	SDL_free(base_path);
	GIS_FreeHolder(gameInputStateHolder);
	Textures_Destroy(textures);
	RL_FreeCreaturesList(rtc->creatures);
	RL_DestroyDebug();
	RL_DestroyPlayer(rtc->player);
	rtc->map->DestroyMap(rtc->map, rtc->conf->mapsizex);
	RL_DestroyMapGeneratorHolder(rtc->mapGenHolder);
	RL_FreeGenerator(rtc->gen);
	RL_FreeConfig(rtc->conf);
	RL_FreeRTContext(rtc);

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}
