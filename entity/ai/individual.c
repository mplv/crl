#include "ai.h"

void AI_IndividualMove(Creature* c, Player* p, Map* m, Random* gen)
{
    int dir = 0;
    dir = gen->GenerateNumber(gen) % 4;
    if (ValidMove(m, p, &c->ent, dir)) {
        EntityMove(&c->ent, dir, m->sizeX, m->sizeY);
    }
}
