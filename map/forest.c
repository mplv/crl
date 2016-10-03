#include "map.h"
#include <string.h>

// give the map some color
void Forest_GetColors(RL_Map *m, unsigned char rgb[3], texture_id_type id)
{
	switch (id) {
		case CAP_T:
		rgb[0] = 0;
		rgb[1] = 218;
		rgb[2] = 0;
		break;
		case EXCLAIMATION:
		rgb[0] = 0;
		rgb[1] = 255;
		rgb[2] = 0;
		break;
		case CAP_I:
		rgb[0] = 0;
		rgb[1] = 161;
		rgb[2] = 84;
		break;
		case CAP_O:
		rgb[0] = 99;
		rgb[1] = 99;
		rgb[2] = 99;
		break;
		case O:
		rgb[0] = 148;
		rgb[1] = 98;
		rgb[2] = 0;
		break;
		default:
		rgb[0] = 255;
		rgb[1] = 255;
		rgb[2] = 255;
		break;
	}
}

// create a map based on the specific generator
void Forest_GenerateMap(RL_Map* m, RL_Generator* gen)
{
	// write out the map to a file for easy viewing of generated map
	// #define DEBUG 1
	#ifdef DEBUG
		FILE *fp = fopen("map.txt","w+");
	#endif
	int floor_count = (m->sizeX * m->sizeY) * 0.8;
	int size_x_half = m->sizeX / 2;
	int size_y_half = m->sizeY / 2;
	int i = 0;
	int j = 0;

	// first fill the map with wall tiles
	for (i = 0; i < m->sizeX; i++) {
		for (j = 0; j < m->sizeY; j++) {
			m->map[i][j] = POUND;
		}
	}
	// tree like generator
	// while we have not opened all tiles
	while(floor_count > 0) {

		// get a start point
		int sx = gen->GenerateNumber(gen) % m->sizeX;
		int sy = gen->GenerateNumber(gen) % m->sizeY;

		// if it is a wall
		if (m->map[sx][sy] == POUND) {
			// while we have not hit an open space
			while (m->map[sx][sy] != DOT) {

				// move at most twice
				int count = 5;
				while (count > 0) {

					// set it as an open space
					m->map[sx][sy] = DOT;
					// decrease number of open tiles needed
					floor_count --;
					// get a new start point based off of the one we generated
					sx = sx > size_x_half ? sx - (gen->GenerateNumber(gen) % 2) : sx + (gen->GenerateNumber(gen) % 2);
					sy = sy > size_y_half ? sy - (gen->GenerateNumber(gen) % 2) : sy + (gen->GenerateNumber(gen) % 2);
					// stop if it is open
					if (m->map[sx][sy] == DOT)
						break;
					// decrease number of moves left
					count --;
				}
				// set curretn pos as open space
				m->map[sx][sy] = DOT;

				// move in dir of center
				sx = sx > size_x_half ? sx - 1 : sx + 1;
				sy = sy > size_y_half ? sy - 1 : sy + 1;
			}

			// move the tile in rand direction for three moves
			// then move towards center for one move
			// once it hits center move to next tile
			// also could make it stop if it is empty
		}
	}

	for (i = 0; i < m->sizeX; i++) {
		for (j = 0; j < m->sizeY; j++) {
			if (m->map[i][j] == POUND)
				m->map[i][j] = m->obstacles[gen->GenerateNumber(gen)% m->obstaclesLen];
		}
	}

	#ifdef DEBUG
		char *chars = " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz|_";
		for (i = 0; i < m->sizeY; i++) {
			for (j = 0; j < m->sizeX; j++) {
				fprintf(fp,"%c",chars[m->map[j][i]]);
			}
			fprintf(fp, "\n");
		}
		fclose(fp);
	#endif
	#undef DEBUG

}

// create a map
RL_Map* RL_CreateForestMap(int w, int h, RL_Generator *gen)
{
	RL_Map *m = calloc(1,sizeof(RL_Map));
	if (!m) {
		return NULL;
	}
	m->creatures = AL_New();

    // create the map
    int i = 0;
    // allocate the top level ptr of size w
    m->map = calloc(w,sizeof(texture_id_type*));
    // for each position in w allocate the char[] size h
    // this covers each position on the screen
    for (i = 0; i < w; i++) {
        m->map[i] = calloc(h,sizeof(texture_id_type));
    }

	m->climate = 0;

	m->obstaclesLen = 3;
	m->obstacles = calloc(3, sizeof(texture_id_type));
	m->obstacles[0] = CAP_O;
	m->obstacles[1] = CAP_T;
	m->obstacles[2] = CAP_I;
	m->sizeX = w;
	m->sizeY = h;

	if (gen) {
		memcpy(&m->initalGeneratorState, gen, sizeof(RL_Generator));
	}

	Forest_GenerateMap(m, gen);

	m->ValidMove = &RL_ValidMove;
	m->GetColor = &Forest_GetColors;
	m->DestroyMap = &RL_DestroyMap;
	return m;
}
