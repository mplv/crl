#include <math.h>
#include <stdlib.h>
#include <strings.h>
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

void RL_GenerateMap(RL_RTContext *rtc)
{
    int i = 0;
    int j = 0;
    char tiles[] = "T., 'o!";
    int numTiles = strnlen(tiles,50);
    srand(rtc->conf->gen);
    for (i = 0 ; i < rtc->conf->width; i++)
    {
        for (j = 0 ; j < rtc->conf->height; j++)
        {
            rtc->map[i][j] = tiles[rand()%numTiles];
        }
    }
}
