#ifndef item_h
#define item_h

typedef struct {
    char name[50];
    char desc[50];
    unsigned char type;
    int amount;
    int number;
}RL_Item;

RL_Item *RL_NewItem();
void RL_DestroyItem(RL_Item *i);

#endif /* item_h */
