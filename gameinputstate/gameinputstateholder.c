#include "gameinputstate.h"


static void GISH_SetActive(GameInputStateHolder *gish, InputState_t s) {
	gish->s = s;
}

static void GISH_Register(GameInputStateHolder *gish, GameInputState *gis)
{
	AL_Add(gish->list,gis);
}
static void GISH_Do(GameInputStateHolder* gish, RL_RTContext* context, SDL_Keycode key) {
	GameInputState *gis = AL_Get(gish->list, gish->s);
	gish->s = gis->Do(context, key);
}

GameInputStateHolder* GIS_NewHolder()
{
	GameInputStateHolder* gish = malloc(sizeof(GameInputStateHolder));
	gish->SetActive = &GISH_SetActive;
	gish->Register = &GISH_Register;
	gish->Do = &GISH_Do;
	gish->list = AL_New();
	gish->s = MAIN;
	return gish;
}

void GIS_FreeHolder(GameInputStateHolder *gish) {
	GameInputState *gis = AL_RemoveLast(gish->list);
	while (gis != NULL) {
		gis->Destroy(gis);
		gis = AL_RemoveLast(gish->list);
	}
	AL_Destroy(gish->list);
	free(gish);
}
