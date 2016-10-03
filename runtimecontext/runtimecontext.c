#include <stdlib.h>
#include "runtimecontext.h"
#include "debug/debug.h"
#include "entity/creature/creature.h"

// Create the roguelike context
RL_RTContext *RL_CreateRTContext()
{
    RL_RTContext *rtc = malloc(sizeof(RL_RTContext));;
    return rtc;
}

// free it
void RL_FreeRTContext(RL_RTContext *rtc)
{
    free(rtc);
}

void RL_Save(RL_RTContext *rtc)
{
	RL_DebugMessage(LOG,"Saving data...");
	// RL_CreaturesSave(rtc->creatures);
	RL_MapSave(rtc->map, rtc->conf->base_path);
	// RL_PlayerSave(rtc->player);
	RL_NumGenSave(rtc->gen, rtc->conf->base_path);
	RL_PlayerSave(rtc->player, rtc->conf->base_path);
}

void RL_LoadSave(RL_RTContext *rtc)
{
	RL_DebugMessage(LOG,"Loading previous save");
	rtc->map = RL_MapLoad(rtc->mapGenHolder,rtc->conf->base_path);
	rtc->gen = RL_NumGenLoad(rtc->conf->base_path);
	rtc->map->creatures = RL_CreatureListLoad(rtc->conf->base_path);
	rtc->player = RL_PlayerLoad(rtc->conf->base_path);
}
