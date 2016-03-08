#ifndef entity_h
#define entity_h

typedef struct {
    int x,y;
    int hp;
    int maxHp;
    int mp;
    int maxMp;
    int atk;
    int def;
    int evn;
    int lck;
    int mst;
}RL_Entity;

RL_Entity* RL_NewEntity();
void RL_DestroyEntity(RL_Entity *rtc);
void RL_EntityMove(RL_Entity *ent, int dir, int maxX, int maxY);

#endif /* entity_h */
