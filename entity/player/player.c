#include <stdlib.h>
#include <string.h>
#include "player.h"
#include "item/item.h"
#include "entity/entity.h"
#include "arraylist/arraylist.h"
#include "debug/debug.h"

Player* NewPlayer()
{
    Player *player = calloc(1, sizeof(Player));
    player->ent = NewEntity();
	player->ent->x = 0;
	player->ent->y = 0;
    player->inventory = NewList();
	player->ent->texture = AT;
    return player;
}

void DestroyPlayer(Player *player)
{
    while (ListSize(player->inventory) > 0) {
        Item *i = ListRemoveLast(player->inventory);
        free(i);
    }
    ListDestroy(player->inventory);
    free(player->ent);
    free(player);
}

void PlayerSave(Player *player, const char *base_path){
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
    DebugMessageStart(LOG);
	printf("%s", save_path);
    DebugMessageEnd();
	FILE *f = fopen(save_path, "w");
	if (f) {
		EntitySave(f, player->ent);
		// InventorySave(f, player->inventory);
		fclose(f);
	}
	free(save_path);
}

Player* PlayerLoad(const char *base_path){
	Player *player = calloc(1,sizeof(Player));
	player->ent = calloc(1,sizeof(Entity));
	player->inventory = NewList();
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
    DebugMessageStart(LOG);
	printf("%s", load_path);
    DebugMessageEnd();
	FILE *f = fopen(load_path, "r");
	if (f) {
		EntityLoad(f, player->ent);
		// InventoryLoad(f, player->inventory);
		fclose(f);
	}
	free(load_path);
	return player;
}
