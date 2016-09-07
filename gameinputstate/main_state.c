#include "gameinputstate.h"
#include <SDL2/SDL.h>
#include <stdlib.h>
#include "runtimecontext/runtimecontext.h"
#include "map/map.h"

// an impl of GameInputState for the main screen
InputState_t GIS_main_do(RL_RTContext *rtc, SDL_Keycode code) {
	int valid = 0;
	int dir = 0;
	switch (code) {
		case SDLK_w:
			valid = RL_ValidMove(rtc->map, rtc->player->ent, 0);
			dir = 0;
			break;
		case SDLK_a:
			valid = RL_ValidMove(rtc->map, rtc->player->ent, 2);
			dir = 2;
			break;
		case SDLK_s:
			valid = RL_ValidMove(rtc->map, rtc->player->ent, 1);
			dir = 1;
			break;
		case SDLK_d:
			valid = RL_ValidMove(rtc->map, rtc->player->ent, 3);
			dir = 3;
			break;
		case SDLK_c:
			// create character screen here?
			// or show it?
			// return CHARACTER;
			break;
		case SDLK_i:
			// open inventory screen
			// or show it?
			// return INVENTORY;
			break;
		case SDLK_ESCAPE:
			// cause exit here?
			break;
	}
	if (valid) {
		RL_EntityMove(rtc->player->ent, dir, rtc->conf->mapsizex - 1, rtc->conf->mapsizey - 1);

	}
	return MAIN;
}


// free it up
void GIS_Main_State_Delete(GameInputState *main_state) {
	free(main_state);
}

// creation of main GameInputState
GameInputState *GIS_Main_State_New() {
	GameInputState *main_state = malloc(sizeof(GameInputState));
	main_state->Do = &GIS_main_do;
	main_state->Destroy = &GIS_Main_State_Delete;
	return main_state;
}
