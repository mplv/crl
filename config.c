#include <stdio.h>
#include <stdlib.h>
#include <termbox.h>
#include "config.h"
RL_Config* RL_LoadConfig(const char *configFilePath)
{
    int gen = 0;
    // Open and read the file storing it into the config struct
    RL_Config *c = malloc(sizeof(RL_Config));
    FILE *f = fopen("data/conf","r");
    fscanf(f, "gen: %d\n", &gen);
    c->gen = gen;
    c->width = tb_width();
    c->height = tb_height();
    fclose(f);
    return c;
}

void RL_FreeConfig(RL_Config *conf)
{
    free(conf);
}

