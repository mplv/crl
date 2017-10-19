#ifndef TEXTURES_H
#define TEXTURES_H

#include <SDL2/SDL.h>
#include "arraylist/arraylist.h"

// texture position names
// chars = ` !"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz|\u2015`
typedef enum {
	SPACE = 0,
	EXCLAIMATION,
	DOUBLE_QUOTE,
	POUND,
	DOLLAR_SIGN,
	PERCENT,
	AMPERSAND,
	SINGLE_QUOTE,
	OPEN_PAREN,
	CLOSE_PAREN,
	STAR,
	PLUS,
	COMMA,
	MINUS,
	DOT,
	BACK_SLASH,
	ZERO,
	ONE,
	TWO,
	THREE,
	FOUR,
	FIVE,
	SIX,
	SEVEN,
	EIGHT,
	NINE,
	COLON,
	SEMICOLON,
	LESS_THAN,
	EQUAL,
	GREATER_THAN,
	QUESTION_MARK,
	AT,
	CAP_A,
	CAP_B,
	CAP_C,
	CAP_D,
	CAP_E,
	CAP_F,
	CAP_G,
	CAP_H,
	CAP_I,
	CAP_J,
	CAP_K,
	CAP_L,
	CAP_M,
	CAP_N,
	CAP_O,
	CAP_P,
	CAP_Q,
	CAP_R,
	CAP_S,
	CAP_T,
	CAP_U,
	CAP_V,
	CAP_W,
	CAP_X,
	CAP_Y,
	CAP_Z,
	A,
	B,
	C,
	D,
	E,
	F,
	G,
	H,
	I,
	J,
	K,
	L,
	M,
	N,
	O,
	P,
	Q,
	R,
	S,
	T,
	U,
	V,
	W,
	X,
	Y,
	Z,
	VERT_BAR,
	HORIZONTAL_BAR,
	NUM_TEXTURES
} texture_id_type;

// textures obj for easy access to the textures
typedef struct _TEXTURES {
	ArrayList *textures;
	int (*Load)(struct _TEXTURES *tex, const char*, SDL_Renderer*);
	void (*Destroy)(struct _TEXTURES *tex);
	SDL_Texture* (*Get)(struct _TEXTURES *tex, texture_id_type id);
} Textures;

// texture functions
Textures* NewTextures();
int LoadTextures(Textures *tex, const char* tilesetPath, SDL_Renderer* renderer);
void DestroyTextures(Textures *tex);
SDL_Texture* TexturesGet(Textures *tex, texture_id_type id);

#endif
