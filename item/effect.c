#include "effect.h"

#define PERCENT 0
#define FLAT 1

Effect ZeroEffect(){
	Effect e;
	e.percentOrFlat = PERCENT;
	e.hp = 1.0;
	e.mp = 1.0;
	e.atk = 1.0;
	e.def = 1.0;
	e.evn = 1.0;
	e.lck = 1.0;
	e.mst = 1.0;
	e.effect = 0;
	return e;
}

#undef PERCENT
#undef FLAT

Effect RandomEffect(){
	return ZeroEffect();
}
