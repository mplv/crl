#include "map.h"
#include <string.h>

// color the map
void Cave_GetColors(Map *m, unsigned char rgb[3], texture_id_type id)
{
	switch (id) {
		case DOT:
		rgb[0] = 120;
		rgb[1] = 120;
		rgb[2] = 120;
		break;
		case POUND:
		rgb[0] = 230;
		rgb[1] = 230;
		rgb[2] = 230;
		break;
		default:
		rgb[0] = 255;
		rgb[1] = 255;
		rgb[2] = 255;
		break;
	}
}

// generate a map based on the generator
void Cave_GenerateMap(Map* m, Random* gen)
{
	// #define DEBUG 1
	#ifdef DEBUG
		FILE *fp = fopen("map.txt","w+");
	#endif
	int i = 0;
    int j = 0;

	// size of map / 2 to get half way point
	int size_x_half = m->sizeX / 2;
	int size_y_half = m->sizeY / 2;

	// small set of tiles get set to open space
	// these create the bounds
	int dx = gen->GenerateNumber(gen) % m->sizeX;
	int dy = gen->GenerateNumber(gen) % m->sizeY;
	int sx = size_x_half - (dx / 2.0);
	int sy = size_y_half - (dy / 2.0);

	// how much of the floor should be open
	int floor_count = (m->sizeX * m->sizeY) * 0.3;

	// first fill the map with wall tiles
	for (i = 0; i < m->sizeX; i++) {
		for (j = 0; j < m->sizeY; j++) {
			m->map[i][j] = POUND;
		}
	}


	for (i = sx; i < sx+dx; i++) {
		for (j = sy; j < sy+dy; j++) {
			m->map[i][j] = DOT;
		}
	}

	while (floor_count > 0) {
		// get a random floor tile
		sx = gen->GenerateNumber(gen) % m->sizeX;
		sy = gen->GenerateNumber(gen) % m->sizeY;
		i = 0;
		// while it is a wall
		while (m->map[sx][sy] != DOT || i < 100) {
			i++;
			// get a new tile near this one
			dx = gen->GenerateNumber(gen) % 2;
			if (dx == 0)
			{
				dy = gen->GenerateNumber(gen) % 2;
				if (dy == 0) {
					dx = sx - 1;
				} else {
					dx = sx + 1;
				}
				if (dx < 0)
					dx = 0;
				if (dx >= m->sizeX)
					dx = m->sizeX - 1;
				dy = sy;
			}
			else
			{
				dx = gen->GenerateNumber(gen) % 2;
				if (dx == 0) {
					dy = sy - 1;
				} else {
					dy = sy + 1;
				}
				if (dy < 0)
					dy = 0;
				if (dy >= m->sizeY)
					dy = m->sizeY - 1;
				dx = sx;
			}
			// if that is a dot then set this one to a dot
			if (m->map[dx][dy] == DOT)
			{
				m->map[sx][sy] = DOT;
				floor_count --;
				break;
			} else {
				sx = dx;
				sy = dy;
				// sx = sx > size_x_half ? sx - 1 : sx + 1;
				// sy = sy > size_y_half ? sy - 1 : sy + 1;
			}
		}
	}

	// clean up
	for (i = 0; i < m->sizeX; i++) {
		m->map[i][0] = POUND;
		m->map[i][m->sizeY-1] = POUND;
	}
	for (j = 0; j < m->sizeY; j++) {
		m->map[0][j] = POUND;
		m->map[m->sizeX-1][j] = POUND;
	}

	// clean up pillars and other stuff
	// TODO the other stuff
	int wall_count;
	for (i = 1; i < m->sizeX-2; i++) {
		for (j = 1; j < m->sizeY-2; j++) {
			wall_count = 0;
			if (m->map[i][j] == POUND) {
				if (m->map[i-1][j-1] == POUND)
				{
					wall_count += 1;
				}

				if (m->map[i][j-1] == POUND)
				{
					wall_count += 1;
				}

				if (m->map[i+1][j-1] == POUND)
				{
					wall_count += 1;
				}

				if (m->map[i-1][j] == POUND)
				{
					wall_count += 1;
				}

				if (m->map[i+1][j] == POUND)
				{
					wall_count += 1;
				}

				if (m->map[i-1][j+1] == POUND)
				{
					wall_count += 1;
				}

				if (m->map[i][j+1] == POUND)
				{
					wall_count += 1;
				}

				if (m->map[i+1][j+1] == POUND)
				{
					wall_count += 1;
				}

				if (wall_count < 3)
				{
					m->map[i][j] = DOT;
					// restart loop to remove all lone pillars
					i = 1;
					j = 1;
				}
			}
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
}

// create the map
Map* CreateCaveMap(int w, int h, Random *gen)
{
	Map *m = calloc(1,sizeof(Map));
	if (!m) {
		return NULL;
	}
	m->creatures = NewList();
    // create the map
    int i = 0;
    // allocate the top level ptr of size w
    m->map = calloc(w,sizeof(texture_id_type*));
    // for each position in w allocate the char[] size h
    // this covers each position on the screen
    for (i = 0; i < w; i++) {
        m->map[i] = calloc(h,sizeof(texture_id_type));
    }

	m->climate = CAVE;

	m->obstaclesLen = 3;
	m->obstacles = calloc(3, sizeof(texture_id_type));
	m->obstacles[0] = CAP_O;
	m->obstacles[1] = CAP_T;
	m->obstacles[2] = CAP_I;
	m->sizeX = w;
	m->sizeY = h;

	if (gen) {
		memcpy(&m->initalGeneratorState, gen, sizeof(Random));
	}

	Cave_GenerateMap(m, gen);
	return m;
}
