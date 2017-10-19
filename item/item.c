#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "item.h"

Item *NewItem()
{
    Item *i = calloc(1, sizeof(Item));
	i->eff = ZeroEffect();
    return i;
}

void DestroyItem(Item *i)
{
    free(i);
}

Item* ItemFromFile(FILE *fp) {
	Item *i = NewItem();
	i->eff = ZeroEffect();
	if (fp) {
		fscanf(fp, "%s\n", i->name);
		fscanf(fp, "%s\n", i->desc);
		fscanf(fp, "%d\n", (int*)&(i->type));
	}
	return i;
}

ArrayList* LoadBaseItems(char* base_path, char* item_folder)
{
	ArrayList *list = NewList();
	int basePathLen = strnlen(base_path, 100);
	int itemFolderLen = strnlen(item_folder, 20);
	int listFileLen = strnlen("itemlist.txt", 12);
	char *listFileName = "itemlist.txt";
	char *base = calloc(basePathLen+itemFolderLen+51,sizeof(char));
	char *fullpath = calloc(basePathLen+itemFolderLen+listFileLen + 1, sizeof(char));
	int i = 0;
	for (i = 0; i < basePathLen;i++)
	{
		fullpath[i] = base_path[i];
		base[i] = base_path[i];
	}
	for (; i < itemFolderLen+basePathLen; i++) {
		fullpath[i] = item_folder[i-basePathLen];
		base[i] = item_folder[i-basePathLen];
	}
	for (; i < itemFolderLen+basePathLen+listFileLen; i++) {
		fullpath[i] = listFileName[i-itemFolderLen-basePathLen];
	}
	fullpath[i] = '\0';
	i -= listFileLen;
	printf("%s\n",fullpath);
	FILE* f = fopen(fullpath,"r");
	if (f) {
		char *line = NULL;
		size_t len = 0;
		int err = 0;
		err = getline(&line,&len, f);
		while(err != -1) {
			printf("%d\n", err);
			if (err < 50) {
				for (; i < itemFolderLen+basePathLen+err-1; i++) {
					base[i] = line[i-itemFolderLen-basePathLen];
				}
				base[i] = '\0';
				i -= err;
			}
			printf("%s\n",base);
			FILE *fp = fopen(base,"r");
			Item *i = ItemFromFile(fp);
			if (i) {
				AL_Add(list,i);
			} else {
				printf("Bad item: %s", line);
			}
			fclose(fp);
			free(line);
			line = NULL;
			len = 0;
			err = getline(&line,&len, f);
		}
		free(line);
		line = NULL;
		len = 0;
		fclose(f);
	}
	free(base);
	free(fullpath);
	return list;
}

Item *RandomBaseItem(){return NewItem();}
Item *RandomPotionItem(){return NewItem();}
Item *RandomArmorItem(){return NewItem();}
Item *RandomWeaponItem(){return NewItem();}
