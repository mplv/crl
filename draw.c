#include <termbox.h>
#include "draw.h"

void RL_Draw(RL_RTContext *rtc)
{
    // draw our map
    int i = 0;
    int j = 0;
    for (i = 0 ; i < rtc->conf->width; i++)
    {
        for (j = 0 ; j < rtc->conf->height; j++)
        {
            tb_change_cell(i,j, rtc->map[i][j], TB_DEFAULT, TB_DEFAULT);
        }
    }
}
