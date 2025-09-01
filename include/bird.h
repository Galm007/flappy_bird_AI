#ifndef __BIRD_H__
#define __BIRD_H__

#include "nn.h"

#include <raylib.h>
#include <raymath.h>

typedef struct {
	Vector2 pos;
	float vely, rot;
	bool alive;
	int skin;
	float score;
} Bird;

extern Texture2D birdtex[9];
extern Sound wingfx, hitfx;

Bird bird_new();
void bird_free(Bird* bird);

void bird_kill(Bird* bird);
void bird_update(float ft, Bird* bird);
void bird_flap(Bird* bird);
void bird_draw(Bird* bird);

void bird_sort(Bird* population, NeuralNetwork* nns);

#endif
