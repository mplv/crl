#include <stdlib.h>
#include "runtimecontext.h"
#include "debug/debug.h"
#include "entity/creature/creature.h"

// Create the roguelike context
Context *CreateRTContext()
{
    Context *rtc = calloc(1, sizeof(Context));
    return rtc;
}

// free it
void FreeRTContext(Context *rtc)
{
    free(rtc);
}

void Save(Context *rtc)
{
	DebugMessage(LOG,"Saving data...\n");
	// CreaturesSave(rtc->creatures);
	MapSave(rtc->map, rtc->conf->base_path);
	// PlayerSave(rtc->player);
	RandomSave(rtc->gen, rtc->conf->base_path);
	PlayerSave(rtc->player, rtc->conf->base_path);
}

int LoadSave(Context *rtc)
{
	DebugMessage(LOG,"Loading previous save\n");
	rtc->map = MapLoad(rtc->mapGenHolder,rtc->conf->base_path);
    if (rtc->map == NULL) {
        return -1;
    }
	rtc->gen = RandomLoad(rtc->conf->base_path);
	rtc->map->creatures = CreatureListLoad(rtc->conf->base_path);
	rtc->player = PlayerLoad(rtc->conf->base_path);
    return 0;
}
