#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "config.h"

RL_Config* RL_LoadConfig(const char *base_path, const char *configFilePath)
{
	// the options that the conf file contains
    int gen = 0;
    int tileWidth = 0;
    int tileHeight = 0;
	int windowWidth = 0;
	int windowHeight = 0;
	char *path;
	int base_path_len = 0;
	int configFilePathLen = 0;
	int i = 0;

    // Open and read the file storing it into the config struct
    RL_Config *c = malloc(sizeof(RL_Config));

	// construct path to config file
	base_path_len = strnlen(base_path,256);
	configFilePathLen = strnlen(configFilePath,256);
	path = calloc(base_path_len+configFilePathLen+1, sizeof(char));
	for (i = 0; i < base_path_len; i++) {
		path[i] = base_path[i];
	}
	for (i = 0; i < configFilePathLen; i++) {
		path[i+base_path_len] = configFilePath[i];
	}
	path[base_path_len+configFilePathLen] = '\0';
	c->path = path;
	c->base_path = base_path;

	// open the file
    FILE *f = fopen(path,"r");
	if (f == NULL) {
		free(c);
		return NULL;
	}
	// read the data from the file
	fscanf(f, "windowWidth: %d\n", &windowWidth);
    fscanf(f, "windowHeight: %d\n", &windowHeight);
	fscanf(f, "tileWidth: %d\n", &tileWidth);
    fscanf(f, "tileHeight: %d\n", &tileHeight);
    fscanf(f, "gen: %d\n", &gen);
	// store data into struct
    c->gen = gen;
    c->width = windowWidth;
    c->height = windowHeight;
	c->tileWidth = tileWidth;
	c->tileHeight = tileHeight;
	c->widthToTiles = c->width / tileWidth;
	c->heightToTiles = c->height / tileHeight;

	// TODO need to set the below in file or some other thing
	//
    // number of tiles in the x and y direction
    c->mapsizex = 75;
    c->mapsizey = 50;
	// close the file
    fclose(f);
    return c;
}

void SaveConf(RL_Config *conf)
{
	// open the conf file
	FILE *f = fopen(conf->path,"w");
	if (f == NULL) {
		return;
	}

	// write all of the options to the file
	fprintf(f, "windowWidth: %d\n", conf->width);
	fprintf(f, "windowHeight: %d\n", conf->height);
	fprintf(f, "tileWidth: %d\n", conf->tileWidth);
    fprintf(f, "tileHeight: %d\n", conf->tileHeight);
    fprintf(f, "gen: %d\n", conf->gen);

	// close the file
	fclose(f);
}

void RL_FreeConfig(RL_Config *conf)
{
	// save all options that might have changed
	SaveConf(conf);
	free(conf->path);
    free(conf);
}
