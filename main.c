#include <termbox.h>
#include <stdio.h>
#include <stdlib.h>
#include "runtimecontext.h"
#include "config.h"
#include "creature.h"
#include "map.h"
#include "draw.h"
#include "arraylist.h"
#include "player.h"

int main (int argc, const char *argv[])
{
    int ret = 0;
    int x,y;
    volatile int running = 1;
    ret = tb_init();
    if (ret) {
        printf("Termbox exited with error code: %d\n",ret);
        return ret;
    }
    tb_select_input_mode(TB_INPUT_ESC);
    struct tb_event ev;
    RL_RTContext *rtc = RL_CreateRTContext();
    rtc->conf = RL_LoadConfig("data/conf");
    RL_CreateMap(rtc, rtc->conf->width, rtc->conf->height);
    if (!rtc->map)
    {
        tb_shutdown();
        printf("Map could not be allocated quitting...");
        return -1;
    }
    rtc->creatures = RL_LoadAllCreatures(rtc->conf);
    x = tb_width();
    y = tb_height();
    RL_GenerateMap(rtc);
    rtc->player = RL_NewPlayer();
    tb_clear();
    RL_Draw(rtc);
    tb_present();
    while (running && tb_poll_event(&ev)) {
        switch (ev.type) {
        case TB_EVENT_KEY:
            if (ev.key == TB_KEY_CTRL_Q) {
                running = 0;
            }
                if (ev.ch != 0) {
                    switch (ev.ch) {
                        case 'w':
                            rtc->player->y > 0 ? rtc->player->y--:0;
                            break;
                        case 'a':
                            rtc->player->x > 0 ? rtc->player->x--:0;
                            break;
                        case 's':
                            rtc->player->y < rtc->conf->height - rtc->conf->bbarwidth - 1? rtc->player->y++:rtc->conf->height - rtc->conf->bbarwidth - 1;
                            break;
                        case 'd':
                            rtc->player->x < rtc->conf->width - rtc->conf->rbarwidth - 1? rtc->player->x++:rtc->conf->width - rtc->conf->rbarwidth - 1;
                            break;
                    }
                }
            break;
        case TB_EVENT_RESIZE:
            tb_clear();
            RL_Draw(rtc);
            tb_present();
            break;
        default:
            break;
        }
        tb_clear();
        RL_Draw(rtc);
        tb_present();
    }
    RL_DestroyPlayer(rtc->player);
    RL_DestroyMap(rtc, tb_width());
    RL_FreeCreaturesList(rtc->creatures);
    AL_Destroy(rtc->creatures);
    RL_FreeConfig(rtc->conf);
    RL_FreeRTContext(rtc);
    tb_shutdown();
    return 0;
}

