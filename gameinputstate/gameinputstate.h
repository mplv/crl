#ifndef GAME_INPUT_STATE_H
#define GAME_INPUT_STATE_H

#include "runtimecontext/runtimecontext.h"
#include "arraylist/arraylist.h"
#include <SDL2/SDL.h>

typedef enum {
	MAIN,
	NUM_STATES
}InputState_t;

// GameInputState for controlling input on various screens
typedef struct _GAME_INPUT_STATE {
	InputState_t (*Do)(Context*, SDL_Keycode);
	void (*Destroy)(struct _GAME_INPUT_STATE*);
} GameInputState;

typedef struct _GAME_INPUT_STATE_HOLDER {
	InputState_t s;
	void (*SetActive)(struct _GAME_INPUT_STATE_HOLDER *,InputState_t);
	void (*Register)(struct _GAME_INPUT_STATE_HOLDER *,GameInputState*);
	void (*Do) (struct _GAME_INPUT_STATE_HOLDER*, Context*, SDL_Keycode);
	ArrayList *list;
} GameInputStateHolder;

// holder functions
GameInputStateHolder* GIS_NewHolder();
void GIS_FreeHolder(GameInputStateHolder *gish);

// each new state constructor below
GameInputState *GIS_MainStateNew();


#endif
