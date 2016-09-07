#include "draw.h"
#include <SDL2/SDL.h>
#include "textures/textures.h"
#include "runtimecontext/runtimecontext.h"

/* For testing that the sprite sheet works with the enum */ /*
void RL_Draw(RL_RTContext *rtc, SDL_Renderer *renderer) {
	texture_id_type i = SPACE;
	int x = 0,y = 0;
	SDL_Texture *tex = NULL;
	SDL_Rect dstRect = {x:0,y:0,w:rtc->conf->tileWidth,h:rtc->conf->tileHeight};
	for (; i < 87; i++) {
		if (x > rtc->conf->widthToTiles - 1)
		{
			x = 0;
			y++;
		}
		dstRect.x = x*64;
		dstRect.y = y*64;
		printf("Getting %d at (%d,%d)\n", i,dstRect.x, dstRect.y);
		tex = Textures_Get(rtc->textures,i);
		if (tex == NULL) {
			printf("Texture %d not availible\n", i);
			break;
		}
		SDL_SetTextureColorMod(tex,255,255,255);
		SDL_RenderCopy(renderer,tex,NULL,&dstRect);
		x++;
	}
	printf("%d\n", O);
} */

void RL_Draw(RL_RTContext *rtc, SDL_Renderer *renderer) {
	int cx = 0;
	int cy = 0;

	// TODO camera went one too far in y direction showed black bar at bottom

	// Calculate the top left x position of the camera
	if (rtc->player->ent->x < (rtc->conf->widthToTiles / 2)) {
		cx = 0;
	}
	else if (rtc->player->ent->x > rtc->conf->mapsizex - (rtc->conf->widthToTiles / 2)) {
		cx = rtc->conf->mapsizex - rtc->conf->widthToTiles;
	}
	else {
		cx = rtc->player->ent->x - (rtc->conf->widthToTiles / 2);
	}

	// same with the y pos
	if (rtc->player->ent->y < (rtc->conf->heightToTiles / 2)) {
		cy = 0;
	}
	else if (rtc->player->ent->y > rtc->conf->mapsizey - (rtc->conf->heightToTiles / 2)) {
		cy = rtc->conf->mapsizey - rtc->conf->heightToTiles;
	}
	else {
		cy = rtc->player->ent->y - (rtc->conf->heightToTiles / 2);
	}

	// draw the map from that location
	SDL_Rect dstRect = {x:0,y:0,w:rtc->conf->tileWidth,h:rtc->conf->tileHeight};
	SDL_Texture *tex = NULL;
	unsigned char rgbMod[3] = {0};
	int i = 0;
	int j = 0;
	for (i = cy; i < cy+rtc->conf->heightToTiles; i++) {
		for (j = cx; j < cx+rtc->conf->widthToTiles; j++) {
			dstRect.x = j*rtc->conf->tileWidth-cx*rtc->conf->tileWidth;
			dstRect.y = i*rtc->conf->tileHeight-cy*rtc->conf->tileHeight;
			tex = Textures_Get(rtc->textures,rtc->map->map[j][i]);
			if (tex == NULL) {
				printf("Texture %d not availible\n", rtc->map->map[j][i]);
				break;
			}
			rtc->map->GetColor(rtc->map,rgbMod,rtc->map->map[j][i]);
			SDL_SetTextureColorMod(tex,rgbMod[0],rgbMod[1],rgbMod[2]);
			SDL_RenderCopy(renderer,tex,NULL,&dstRect);
			SDL_SetTextureColorMod(tex,255,255,255);
		}
	}

	// for _, e := range GameState.MonsterList {}
	int numCreatures = AL_Size(rtc->creatures);
	for (i = 0; i < numCreatures; i++) {
		// draw creatures and color them
		// assuming that they are visible in the camera
	}

	// blank out the tile behind the player
	dstRect.x = (rtc->player->ent->x - cx) * rtc->conf->tileWidth;
	dstRect.y = (rtc->player->ent->y - cy) * rtc->conf->tileWidth;
	SDL_SetRenderDrawColor(renderer,0,0,0,255);
	SDL_RenderFillRect(renderer, &dstRect);
	// then draw the character offset by the camera
	// tex = textures.GetTile('@')
	dstRect.x = (rtc->player->ent->x - cx) * rtc->conf->tileWidth;
	dstRect.y = (rtc->player->ent->y - cy) * rtc->conf->tileWidth;
	tex = Textures_Get(rtc->textures,AT);
	if (tex == NULL) {
		printf("Texture %d not availible\n", AT);
	}
	SDL_SetTextureColorMod(tex,rtc->player->ent->r,rtc->player->ent->g,rtc->player->ent->b);
	SDL_RenderCopy(renderer,tex,NULL,&dstRect);
	SDL_SetTextureColorMod(tex,255,255,255);
}
