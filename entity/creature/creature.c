#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include "config/config.h"
#include "creature.h"
#include "arraylist/arraylist.h"
#include "debug/debug.h"

// print out the creature for memory corruption or loading issues
void RL_PrintCreature(RL_Creature *c)
{
    printf("%s:\n\t",c->name);
    printf("pre: %d\n\t",c->pre);
	printf("hp: %d\n",c->ent.hp);
	printf("atk: %d\n",c->ent.atk);
	printf("def: %d\n",c->ent.def);
	printf("evn: %d\n",c->ent.evn);
	printf("lck: %d\n",c->ent.lck);
    printf("droptable: %d\n\t",c->droptable);
    printf("climate: %c\n\t",c->climate);
    printf("r,g,b: %u,%u,%u\n",c->ent.r,c->ent.g,c->ent.b);
}

// load the creature from a file
RL_Creature* RL_LoadCreatureDef(char *filestr)
{
    // allocate the creature
    RL_Creature *c = calloc(1,sizeof(RL_Creature));
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
        fscanf(f, "climate: %c\n",&c->climate);
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

void RL_CreatureSave(FILE *f, RL_Creature* c){
	fprintf(f, "%s\n", c->name);
	fprintf(f, "%d\n", c->pre);
	RL_EntitySave(f,&c->ent);
	fprintf(f, "%d\n", c->droptable);
	fprintf(f, "%c\n", c->climate);
}

void RL_CreatureListSave(ArrayList* l, const char *base_path)
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
	RL_DebugMessage(LOG, save_path);
	FILE *f = fopen(save_path, "w");
	if (f) {
		fprintf(f, "%d\n", AL_Size(l));
		RL_Creature* creature = AL_RemoveLast(l);
		while (creature != NULL) {
			RL_CreatureSave(f,creature);
			free(creature->name);
			free(creature);
			creature = AL_RemoveLast(l);
		}
		fclose(f);
	}
	free(save_path);
}

void RL_CreatureLoad(FILE *f, RL_Creature* c){
	fgets(c->name, 20, f);
	fscanf(f," ");
	fscanf(f, "%d\n", &c->pre);
	RL_EntityLoad(f,&c->ent);
	fscanf(f, "%d\n", &c->droptable);
	fscanf(f, "%c\n", &c->climate);
}

ArrayList* RL_CreatureListLoad(const char *base_path)
{
	ArrayList *l = AL_New();
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
	RL_DebugMessage(LOG, load_path);
	FILE *f = fopen(load_path, "r");
	if (f) {
		fscanf(f,"%d\n",&num);
		RL_Creature *c;
		while (num > 0) {
			c = calloc(1, sizeof(RL_Creature));
			c->name = calloc(20,sizeof(char));
			RL_CreatureLoad(f,c);
			AL_Add(l,c);
			num--;
		}
		fclose(f);
	}
	free(load_path);
	return l;
}

// only on *nix boxes can we find the creatures
ArrayList* RL_LoadAllCreatures(RL_Config *c)
{
    ArrayList *l = AL_New();
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
                RL_Creature *creature = RL_LoadCreatureDef(path);
                // add the creature to the list
                if (creature)
                {
                    AL_Add(l,creature);
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
void RL_FreeCreaturesList(ArrayList *l)
{
    while (AL_Size(l) > 0) {
        RL_Creature *c = (RL_Creature*)AL_RemoveLast(l);
        free(c);
    }
	AL_Destroy(l);
}
