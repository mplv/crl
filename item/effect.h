#ifndef ITEM_EFFECT
#define ITEM_EFFECT

typedef struct _EFFECT {
	// should be % or flat?
	// 0 for %, 1 for flat
	int percentOrFlat;
	// bonuses to stats
	float hp;
	float mp;
	float atk;
	float def;
	float evn;
	float lck;
	float mst;
	/*
	num		Effect
	0x0		NONE
	0x1		fire
	0x2		lightning
	0x4		ice
	0x8		steal health
	0x10	steal mp
	Note: more than one can apply up to a max of 2
	*/
	int effect;
} Effect;

Effect ZeroEffect();
Effect RandomEffect();

#endif
