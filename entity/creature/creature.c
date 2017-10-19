#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include "config/config.h"
#include "creature.h"
#include "arraylist/arraylist.h"
#include "debug/debug.h"

// print out the creature for memory corruption or loading issues
void PrintCreature(Creature *c)
{
    printf("%s:\n\t",c->name);
    printf("pre: %d\n\t",c->pre);
	printf("hp: %d\n",c->ent.hp);
	printf("atk: %d\n",c->ent.atk);
	printf("def: %d\n",c->ent.def);
	printf("evn: %d\n",c->ent.evn);
	printf("lck: %d\n",c->ent.lck);
    printf("droptable: %d\n\t",c->droptable);
    printf("climate: %d\n\t",c->climate);
	printf("ai: %c\n\t",c->ai);
    printf("r,g,b: %u,%u,%u\n",c->ent.r,c->ent.g,c->ent.b);
}

// load the creature from a file
Creature* LoadCreatureDef(char *filestr)
{
    // allocate the creature
    Creature *c = calloc(1,sizeof(Creature));
	c->name = calloc(20,sizeof(char));
    FILE *f;
    f = fopen(filestr, "r");
    if (f)
    {
		int tex = 0;
        // using the data in the file load the creature
        fscanf(f, "%s\n",c->name);
        fscanf(f, "pre: %d\n",&c->pre);
		fscanf(f, "hp: %d\n",&c->ent.hp);
        fscanf(f, "atk: %d\n",&c->ent.atk);
        fscanf(f, "def: %d\n",&c->ent.def);
        fscanf(f, "evn: %d\n",&c->ent.evn);
        fscanf(f, "lck: %d\n",&c->ent.lck);
		fscanf(f, "texture: %d\n",&tex);
        fscanf(f, "droptable: %d\n",&c->droptable);
        fscanf(f, "climate: %d\n",(int*)(&c->climate));
		fscanf(f, "ai: %c", &c->ai);
        fscanf(f, "color: %u,%u,%u\n",&c->ent.r,&c->ent.g,&c->ent.b);
		c->ent.maxHp = c->ent.hp;
		c->ent.texture = tex;
        // do things is pre is > version
    }
    else
    {
        free(c);
        fclose(f);
        printf("Failed to load: %s!\n",filestr);
        return NULL;
    }
    if (f != NULL)
    {
        fclose(f);
    }
    return c;
}

void CreatureSave(FILE *f, Creature* c){
	fprintf(f, "%s\n", c->name);
	fprintf(f, "%d\n", c->pre);
	EntitySave(f,&c->ent);
	fprintf(f, "%d\n", c->droptable);
	fprintf(f, "%d\n", c->climate);
	fprintf(f, "%c\n", c->ai);
}

void CreatureListSave(ArrayList* l, const char *base_path)
{
	const char *save_loc = "data/save/creatures.txt";
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
		fprintf(f, "%d\n", ListSize(l));
		i = 0;
		int size = ListSize(l);
		for(;i<size;i++)
		{
			CreatureSave(f,ListGet(l,i));
		}
		fclose(f);
	}
	free(save_path);
}

void CreatureLoad(FILE *f, Creature* c){
	fgets(c->name, 20, f);
	fscanf(f," ");
	fscanf(f, "%d\n", &c->pre);
	EntityLoad(f,&c->ent);
	fscanf(f, "%d\n", &c->droptable);
	fscanf(f, "%d\n", (int*)(&c->climate));
	fscanf(f, "%c\n", &c->ai);
}

ArrayList* CreatureListLoad(const char *base_path)
{
	ArrayList *l = NewList();
	const char *load_loc = "data/save/creatures.txt";
	int basePathLen = 0;
	int loadLocLen = 0;
	int i = 0;
	int num = 0;
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
		fscanf(f,"%d\n",&num);
		Creature *c;
		while (num > 0) {
			c = calloc(1, sizeof(Creature));
			c->name = calloc(20,sizeof(char));
			c->nameAllocd = 1;
			CreatureLoad(f,c);
			ListAdd(l,c);
			num--;
		}
		fclose(f);
	}
	free(load_path);
	return l;
}

// only on *nix boxes can we find the creatures
ArrayList* LoadAllCreatures(Config *c)
{
    ArrayList *l = NewList();
    // open the directory holding the creatures
    DIR *d = NULL;
    struct dirent *dir;
    // use the path to load the creatures
    char path[47] = "data/creatures/";
    d = opendir("data/creatures");
    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
            // make sure we do not load the . and .. files
            if (strncmp(".",dir->d_name,1) != 0)
            {
                // modify the path so that we can load the creatures
                strncpy(path + 15, dir->d_name, 32);
                Creature *creature = LoadCreatureDef(path);
                // add the creature to the list
                if (creature)
                {
                    ListAdd(l,creature);
                }
				else {
					closedir(d);
					return NULL;
				}
            }
        }
        // clean up
        closedir(d);
        return l;
    }

    return NULL;
}

// free all of the creatures stored in the creature arraylist
void FreeCreaturesList(ArrayList *l)
{
    while (ListSize(l) > 0) {
        Creature *c = (Creature*)ListRemoveLast(l);
		free(c->name);
        free(c);
    }
	ListDestroy(l);
}
