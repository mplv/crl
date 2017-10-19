#ifndef item_h
#define item_h

#include "effect.h"
#include "arraylist/arraylist.h"

typedef enum _ITEM_TYPE {
	I_POTION = 0,

	I_ARMOR_CHEST,
	I_ARMOR_LEGS,
	I_ARMOR_SHOULDERS,
	I_ARMOR_GLOVES,
	I_ARMOR_BOOTS,
	I_ARMOR_BELT,

	I_WEAPON_SWORD,
	I_WEAPON_RAPIER,
	I_WEAPON_DAGGER,
	I_WEAPON_AXE,
	I_WEAPON_MACE,

	I_WEAPON_GREATSWORD,
	I_WEAPON_GREATAXE,
	I_WEAPON_MAUL,

	I_WEAPON_STAFF,
	I_WEAPON_BOW,
} ItemType;

typedef struct {
    char name[50];	// name of item
    char desc[50];	// description of item
	Effect eff;		// Effect the item has on stats
	ItemType type;	// type of item
}Item;

ArrayList* LoadBaseItems(char* base_path, char* item_folder);
Item *NewItem();
Item *RandomItem();
void DestroyItem(Item *i);

#endif /* item_h */
