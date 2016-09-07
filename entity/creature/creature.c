#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include "config/config.h"
#include "creature.h"
#include "arraylist/arraylist.h"

// print out the creature for memory corruption or loading issues
void RL_PrintCreature(RL_Creature *c)
{
    printf("%s:\n\t",c->name);
    printf("pre: %d\n\t",c->pre);
    printf("atk: %d\n\t",c->atk);
    printf("def: %d\n\t",c->def);
    printf("evn: %d\n\t",c->evn);
    printf("lck: %d\n\t",c->lck);
    printf("droptable: %d\n\t",c->droptable);
    printf("climate: %c\n\t",c->climate);
    printf("r,g,b: %u,%u,%u\n",c->r,c->g,c->b);
}

// load the creature from a file
RL_Creature* RL_LoadCreatureDef(char *filestr)
{
    // allocate the creature
    RL_Creature *c = calloc(1,sizeof(RL_Creature));
    FILE *f;
    f = fopen(filestr, "r");
    if (f)
    {
		// create locations for the colors
		unsigned int r;
		unsigned int g;
		unsigned int b;
        // using the data in the file load the creature
        fscanf(f, "%s\n",c->name);
        fscanf(f, "pre: %d\n",&c->pre);
        fscanf(f, "atk: %d\n",&c->atk);
        fscanf(f, "def: %d\n",&c->def);
        fscanf(f, "evn: %d\n",&c->evn);
        fscanf(f, "lck: %d\n",&c->lck);
        fscanf(f, "droptable: %d\n",&c->droptable);
        fscanf(f, "climate: %c\n",&c->climate);
        fscanf(f, "color: %u,%u,%u\n",&r,&g,&b);
		c->r = r;
		c->g = g;
		c->b = b;
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
