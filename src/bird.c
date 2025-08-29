#include "bird.h"
#include "constants.h"
#include "sprite.h"
#include "game_state.h"

#include <stdbool.h>
#include <raylib.h>

#define ANIMATION_FRAME_TIME 0.1

int animation_frame = 0;
Texture2D birdtex[9];
Sound wingfx, hitfx, diefx;

Bird bird_new() {
	Bird bird = {
		.pos = (Vector2) { SCREEN_WIDTH / 4.0f, GetRandomValue(200, SCREEN_HEIGHT - 200) },
		.vely = 0.0f,
		.rot = 0.0f,
		.alive = true,
		.skin = GetRandomValue(0, 2),
		.score = 0.0f
	};

	return bird;
}

void bird_free(Bird* bird) {
	bird->pos = Vector2Zero();
	bird->vely = bird->rot = 0.0f;
	bird->alive = false;
}

void bird_kill(Bird* bird) {
	bird->alive = false;
	bird->vely = -JUMP_FORCE * 0.8f;
	PlaySound(hitfx);
}

void bird_update(float ft, Bird* bird) {
	if (bird->alive) {
		bird->score += (score + 1);
	}

	// gravity
	bird->vely += GRAVITY * ft;
	bird->pos.y += bird->vely * ft;
	bird->rot = bird->vely * 0.05f;
	if (bird->pos.y < 0.0f) {
		bird->pos.y = 0.0f;
	}

	// animation
	static float animation_timer = 0.0f;
	animation_timer -= ft;
	while (animation_timer <= 0.0f) {
		animation_timer += ANIMATION_FRAME_TIME;
		animation_frame = (animation_frame + 1) % 3;
	}

	// kill bird if it touched ground
	Rectangle col = get_collider(bird->pos, &birdtex[0]);
	if (bird->alive && col.y + col.height / 2.0f > SCREEN_HEIGHT - GROUND_HEIGHT)
		bird_kill(bird);
}

void bird_flap(Bird* bird) {
	bird->vely = -JUMP_FORCE;
	PlaySound(wingfx);
}

void bird_draw(Bird* bird) {
	draw_sprite(&birdtex[animation_frame + 3 * bird->skin], bird->pos, bird->rot, false);
}

// ugh
static void swap_bird(Bird* arr, int a, int b) {
	Bird tmp = arr[a];
	arr[a] = arr[b];
	arr[b] = tmp;
}
static void swap_nn(NeuralNetwork* arr, int a, int b) {
	NeuralNetwork tmp = arr[a];
	arr[a] = arr[b];
	arr[b] = tmp;
}
void bird_sort(Bird* population, NeuralNetwork* nns) {
	// bubble sort
	for (;;) {
		bool sorted = true;
		for (int i = 0; i < POPULATION - 1; i++) {
			if (population[i].score < population[i + 1].score) {
				swap_bird(population, i, i + 1);
				swap_nn(nns, i, i + 1);
				sorted = false;
			}
		}
		if (sorted)
			break;
	}
}
