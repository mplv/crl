#include "textures.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "arraylist/arraylist.h"
#include "debug/debug.h"

// create the texture holder
Textures* NewTextures() {
	Textures* t = calloc(1, sizeof(Textures));
	if (t == NULL)
	{
		return NULL;
	}
	t->textures = NewList();
	if (t->textures == NULL) {
		free(t);
		return NULL;
	}
	t->Load = &LoadTextures;
	t->Destroy = &DestroyTextures;
	t->Get = &TexturesGet;
	return t;
}

// LoadTextures returns the number of textures created
int LoadTextures(Textures *tex, const char* tilesetPath, SDL_Renderer* renderer) {

	// open texture file should open from tilesetPath but not yet
	char path[500] = {0};
	// make the correct path
	int lenOfPath = strnlen(tilesetPath, 500);
	strncpy(path,tilesetPath,500);
	strncpy(path+lenOfPath,"img/tileset.png", 15);
	// load the texturemap
	SDL_Surface *bitmapSurface = IMG_Load(path);
	if (bitmapSurface == NULL) {
		printf("Error opening game textures. Please make sure they exist\n");
		return -1;
	}

	// pick out the textures we want
	SDL_Rect srcRect= {w:64,h:64};
	int numTextures = (5*16) + 8; // 26*2+35;

	// get all of the textures and make them into sdl textures
	int i = 0;
	for (i = 0; i < numTextures; i++) {
		// create a surface so that we can blit the tile to it
		SDL_Surface *surface = SDL_CreateRGBSurface(0,64,64, bitmapSurface->format->BitsPerPixel,
			bitmapSurface->format->Rmask, bitmapSurface->format->Gmask, bitmapSurface->format->Bmask,bitmapSurface->format->Amask);
		if (surface == NULL) {
			printf("Could not create surface\n");
			SDL_FreeSurface(bitmapSurface);
			return -1;
		}

		// find out the coords of the tile
		if (i < 16) {
			srcRect.x = 64 * i;
			srcRect.y = 0;
		}
		else if (i < 32) {
			srcRect.x = 64 * (i - 16);
			srcRect.y = 64;
		}
		else if (i < 48) {
			srcRect.x = 64 * (i - 32);
			srcRect.y = 64 * 2;
		}
		else if (i < 64) {
			srcRect.x = 64 * (i - 48);
			srcRect.y = 64 * 3;
		}
		else if (i < 80) {
			srcRect.x = 64 * (i - 64);
			srcRect.y = 64 * 4;
		}
		else if (i < 96) {
			srcRect.x = 64 * (i - 80);
			srcRect.y = 64 * 5;
		}

		// blit the tile to the surface
		int blitWorked = SDL_BlitSurface(bitmapSurface,&srcRect,surface,NULL);
		if (blitWorked < 0) {
			printf(
				"\nCould not blit surfaces:  %s\n",
				SDL_GetError()
			);
			SDL_FreeSurface(bitmapSurface);
			SDL_FreeSurface(surface);
			return -1;
		}

		// create a texture from the surface
		SDL_Texture *tmp_tex = SDL_CreateTextureFromSurface(renderer,surface);
		if (tmp_tex == NULL) {
			printf(
				"\nCould not create texture:  %s\n",
				SDL_GetError()
			);
			SDL_FreeSurface(surface);
			SDL_FreeSurface(bitmapSurface);
			return -1;
		}

		// add the new texture to the texture list
		ListAdd(tex->textures, tmp_tex);
		// clean up the surface
		SDL_FreeSurface(surface);
	}
	// free up the tileset
	SDL_FreeSurface(bitmapSurface);

	// return the number of textures
	return ListSize(tex->textures);
}


void DestroyTextures(Textures *tex){
	// remove all of the textures from the list
	SDL_Texture* texture = ListRemoveLast(tex->textures);
	while (texture != NULL)
	{
		SDL_DestroyTexture(texture);
		texture = ListRemoveLast(tex->textures);
	}
	// free the list
	// note only works if the list size is 0
	ListDestroy(tex->textures);
	// free the container
	free(tex);
}

// get the specific texture requested
SDL_Texture* TexturesGet(Textures *tex, texture_id_type id){
	return ListGet(tex->textures, id);
}
