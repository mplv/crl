#include "gameinputstate.h"


static void GISH_SetActive(GameInputStateHolder *gish, InputState_t s) {
	gish->s = s;
}

static void GISH_Register(GameInputStateHolder *gish, GameInputState *gis)
{
	ListAdd(gish->list,gis);
}
static void GISH_Do(GameInputStateHolder* gish, Context* context, SDL_Keycode key) {
	GameInputState *gis = ListGet(gish->list, gish->s);
	gish->s = gis->Do(context, key);
}

GameInputStateHolder* GIS_NewHolder()
{
	GameInputStateHolder* gish = malloc(sizeof(GameInputStateHolder));
	gish->SetActive = &GISH_SetActive;
	gish->Register = &GISH_Register;
	gish->Do = &GISH_Do;
	gish->list = NewList();
	gish->s = MAIN;
	return gish;
}

void GIS_FreeHolder(GameInputStateHolder *gish) {
	GameInputState *gis = ListRemoveLast(gish->list);
	while (gis != NULL) {
		gis->Destroy(gis);
		gis = ListRemoveLast(gish->list);
	}
	ListDestroy(gish->list);
	free(gish);
}
