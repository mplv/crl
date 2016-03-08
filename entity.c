#include <stdlib.h>
#include <stdio.h>
#include "entity.h"
#include "debug.h"

RL_Entity* RL_NewEntity()
{
    RL_Entity *ent = calloc(1, sizeof(RL_Entity));
    ent->hp = 50;
    ent->maxHp = 90;
    ent->mp = 83;
    ent->maxMp = 137;
    return ent;
}

void RL_DestroyEntity(RL_Entity *ent)
{
    free(ent);
}

void RL_EntityMove(RL_Entity *ent, int dir, int maxX, int maxY)
{
    char message[80];
    
    switch (dir) {
        case 0: // w
            ent->y -= 1;
            break;
        case 1: // s
            ent->y += 1;
            break;
        case 2: // a
            ent->x -= 1;
            break;
        case 3: // d
            ent->x += 1;
            break;
    }
    ent->x = ent->x < 0 ? 0 : ent->x;
    ent->y = ent->y < 0 ? 0 : ent->y;
    
    if (ent->x > maxX)
    {
        ent->x = maxX;
    }
    
    if (ent->y > maxY)
    {
        ent->y = maxY;
    }
    
    snprintf(message, 80, "char at: %d, %d", ent->x, ent->y);
    RL_AddDebugMessage(message);
    
}
