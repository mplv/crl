#include <stdlib.h>
#include <string.h>
#include "player.h"
#include "item/item.h"
#include "entity/entity.h"
#include "arraylist/arraylist.h"
#include "debug/debug.h"

RL_Player* RL_NewPlayer()
{
    RL_Player *player = calloc(1, sizeof(RL_Player));
    player->ent = RL_NewEntity();
	player->ent->x = 0;
	player->ent->y = 0;
    player->inventory = AL_New();
	player->ent->texture = AT;
    return player;
}

void RL_DestroyPlayer(RL_Player *player)
{
    while (AL_Size(player->inventory) > 0) {
        RL_Item *i = AL_RemoveLast(player->inventory);
        free(i);
    }
    AL_Destroy(player->inventory);
    free(player->ent);
    free(player);
}

void RL_PlayerSave(RL_Player *player, const char *base_path){
	const char *save_loc = "data/save/player.txt";
	int basePathLen = 0;
	int saveLocLen = 0;
	int i = 0;
	basePathLen = strnlen(base_path,256);
	saveLocLen = strnlen(save_loc,256);
	char *save_path = calloc((basePathLen + saveLocLen + 1), sizeof(char));
	for (i = 0; i < basePathLen; i++) {
		save_path[i] = base_path[i];
	}
	for (i = 0; i < saveLocLen; i++) {
		save_path[i + basePathLen] = save_loc[i];
	}
	save_path[basePathLen+saveLocLen] = '\0';
	RL_DebugMessage(LOG, save_path);
	FILE *f = fopen(save_path, "w");
	if (f) {
		RL_EntitySave(f, player->ent);
		// RL_InventorySave(f, player->inventory);
		fclose(f);
	}
	free(save_path);
}

RL_Player* RL_PlayerLoad(const char *base_path){
	RL_Player *player = calloc(1,sizeof(RL_Player));
	player->ent = calloc(1,sizeof(RL_Entity));
	player->inventory = AL_New();
	const char *load_loc = "data/save/player.txt";
	int basePathLen = 0;
	int loadLocLen = 0;
	int i = 0;
	basePathLen = strnlen(base_path,256);
	loadLocLen = strnlen(load_loc,256);
	char *load_path = calloc((basePathLen + loadLocLen + 1), sizeof(char));
	for (i = 0; i < basePathLen; i++) {
		load_path[i] = base_path[i];
	}
	for (i = 0; i < loadLocLen; i++) {
		load_path[i + basePathLen] = load_loc[i];
	}
	load_path[basePathLen+loadLocLen] = '\0';
	RL_DebugMessage(LOG, load_path);
	FILE *f = fopen(load_path, "r");
	if (f) {
		RL_EntityLoad(f, player->ent);
		// RL_InventoryLoad(f, player->inventory);
		fclose(f);
	}
	free(load_path);
	return player;
}
