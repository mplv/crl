#include <termbox.h>
#include <stdio.h>
#include <string.h>
#include "draw.h"

void RL_DrawLowerBar(RL_RTContext *rtc)
{
    int i = 0;
    int j = 0;
    for (j = rtc->conf->height - rtc->conf->bbarwidth + 1; j < rtc->conf->height; j++)
    {
        for (i = 0 ; i < rtc->conf->width - rtc->conf->rbarwidth; i++)
        {
            tb_change_cell(i,j, ' ', TB_DEFAULT, TB_DEFAULT);
        }
    }
    for (i = 0 ; i < rtc->conf->width; i++)
    {
        tb_change_cell(i,rtc->conf->height - rtc->conf->bbarwidth, '-', TB_DEFAULT, TB_DEFAULT);
    }
}

void RL_DrawPlayerHealth(RL_RTContext *rtc)
{
    int i = 0;
    int j = 0;
    char full[] = "||||||";
    char shp[] = "HP:";
    char smp[] = "MP:";
    char php[6] = {0};
    char pmp[6] = {0};
    
    strncpy(php,full, (int)(5.0 * (((double)rtc->player->hp) / ((double)rtc->player->maxHp))));
    strncpy(pmp,full, (int)(5.0 * (((double)rtc->player->mp) / ((double)rtc->player->maxMp))));
    
    
    int delta = rtc->conf->width - rtc->conf->rbarwidth +1;
    
    for (j = 0; j < rtc->conf->height - rtc->conf->bbarwidth; j++)
    {
        for (i = rtc->conf->width - rtc->conf->rbarwidth +1; i < rtc->conf->width; i++)
        {
            switch (j) {
                case 0:
                    if (i-delta < 3) {
                        tb_change_cell(i,j, shp[i-delta], TB_DEFAULT, TB_DEFAULT);
                    }
                    break;
                case 1:
                    if (i-delta < 7) {
                        if (i-delta == 0) {
                            tb_change_cell(i,j, '[', TB_DEFAULT, TB_DEFAULT);
                        }
                        else if (i-delta == 6) {
                            tb_change_cell(i,j, ']', TB_DEFAULT, TB_DEFAULT);
                        } else {
                            tb_change_cell(i,j, php[i-delta - 1], TB_DEFAULT, TB_DEFAULT);
                        }
                    }
                    break;
                case 3:
                    if (i-delta < 3) {
                        tb_change_cell(i,j, smp[i-delta], TB_DEFAULT, TB_DEFAULT);
                    }
                    break;
                case 4:
                    if (i-delta < 7) {
                        if (i-delta == 0) {
                            tb_change_cell(i,j, '[', TB_DEFAULT, TB_DEFAULT);
                        }
                        else if (i-delta == 6) {
                            tb_change_cell(i,j, ']', TB_DEFAULT, TB_DEFAULT);
                        } else {
                            tb_change_cell(i,j, pmp[i-delta - 1], TB_DEFAULT, TB_DEFAULT);
                        }
                    }
                    break;
                default:
                    tb_change_cell(i,j, ' ', TB_DEFAULT, TB_DEFAULT);
                    break;
            }
        }
    }

}

void RL_DrawRightBar(RL_RTContext *rtc)
{
    RL_DrawPlayerHealth(rtc);
    int i = 0;
    for (i = 0 ; i < rtc->conf->height; i++)
    {
        tb_change_cell(rtc->conf->width - rtc->conf->rbarwidth,i, '|', TB_DEFAULT, TB_DEFAULT);
    }
}

void RL_Draw(RL_RTContext *rtc)
{
    // draw the bars
    RL_DrawRightBar(rtc);
    RL_DrawLowerBar(rtc);
    // finish the bars
    tb_change_cell(rtc->conf->width - rtc->conf->rbarwidth,rtc->conf->height - rtc->conf->bbarwidth, '+', TB_DEFAULT, TB_DEFAULT);
    // draw our map
    int i = 0;
    int j = 0;
    for (i = 0 ; i < rtc->conf->width - rtc->conf->rbarwidth; i++)
    {
        for (j = 0 ; j < rtc->conf->height - rtc->conf->bbarwidth; j++)
        {
            tb_change_cell(i,j, rtc->map[i][j], TB_DEFAULT, TB_DEFAULT);
        }
    }
    tb_change_cell(rtc->player->x,rtc->player->y, '@', TB_DEFAULT, TB_DEFAULT);
}
