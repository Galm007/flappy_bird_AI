#ifndef __PIPES_H__
#define __PIPES_H__

#include <raylib.h>
#include "bird.h"

#define PIPE_COUNT 8
extern Vector2 pipes[PIPE_COUNT];

void initialize_pipes();
void move_pipes(float ft, Texture* pipetex);
void collide_pipes(Bird* bird, Texture* pipetex);
void draw_pipes(Texture* pipetex);

Vector2 next_pipe();

#endif
