#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "map.h"
#include "runtimecontext.h"

void RL_CreateMap(RL_RTContext* rl, int w, int h)
{
    // create the map
    int i = 0;
    // allocate the top level ptr of size w
    rl->map = calloc(w,sizeof(char*));
    // for each position in w allocate the char[] size h
    // this covers each position on the screen
    for (i = 0; i < w; i++) {
        rl->map[i] = calloc(h,sizeof(char));
    }
}

// free the map pointer that we allocated
void RL_DestroyMap(RL_RTContext* rl, int w)
{
    int i = 0;
    for (i = 0; i < w; i++) {
        free(rl->map[i]);
    }
    free(rl->map);
}

int RL_ValidMove(RL_RTContext *rtc, RL_Entity *ent, int dir)
{
    int b = 1;
    int i = 0;
    char tiles[] = "To!";
    int strl = strnlen(tiles,3);
    int x = ent->x;
    int y = ent->y;
    switch (dir) {
        case 0: // w
            y -= 1;
            break;
        case 1: // s
            y += 1;
            break;
        case 2: // a
            x -= 1;
            break;
        case 3: // d
            x += 1;
            break;
    }
    
    if (x < 0) x = 0;
    if (y < 0) y = 0;
    
    if (x > rtc->conf->width - rtc->conf->rbarwidth - 1)
    {
        x = rtc->conf->width - rtc->conf->rbarwidth - 1;
    }
    
    if (y > rtc->conf->height - rtc->conf->bbarwidth - 1)
    {
        y = rtc->conf->height - rtc->conf->bbarwidth - 1;
    }
    
    for (i = 0; i < strl; i++) {
        if (rtc->map[x][y] == tiles[i])
            b = 0;
    }
    return b;
}

void RL_GenerateMap(RL_RTContext *rtc)
{
    int i = 0;
    int j = 0;
    char tiles[] = "T., 'o!";
    int numTiles = strnlen(tiles,50);
    srand(rtc->conf->gen);
    for (i = 0 ; i < rtc->conf->mapsizex; i++)
    {
        for (j = 0 ; j < rtc->conf->mapsizey; j++)
        {
            if (rand()%4  == 0) {
                rtc->map[i][j] = tiles[rand()%numTiles];
            } else {
                rtc->map[i][j] = ' ';
            }
            
        }
    }
}
