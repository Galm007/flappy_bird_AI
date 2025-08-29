#include "constants.h"
#include "game_state.h"
#include "sprite.h"
#include "bird.h"
#include "pipes.h"
#include "nn.h"

#include <raylib.h>
#include <raymath.h>
#include <stdio.h>

/*
 * Inputs:
 * - next pipe ypos (ratio to window height)
 * - bird ypos (ratio to window height)
 * - next pipe xdistance (ratio to pipe spread)
 *
 * NN Structure:
 * 3x2x2x2
 * Flap if the first output node is greater than the second output node
 */

float point_timer = 0.0f;
unsigned int score = 0;
int bg_skin = 0;

int main() {
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Flappy Bird");
	InitAudioDevice();
	SetTargetFPS(60);

	// load textures
	Texture2D bgtex[2] = {
		LoadTexture("../assets/sprites/background-day.png"),
		LoadTexture("../assets/sprites/background-night.png")
	};
	Texture2D pipetex[2] = {
		LoadTexture("../assets/sprites/pipe-green.png"),
		LoadTexture("../assets/sprites/pipe-red.png")
	};
	Texture2D groundtex = LoadTexture("../assets/sprites/base.png");
	Texture2D numbertex[10];
	{
		char filename[64];
		for (int i = 0; i < 10; i++) {
			sprintf(filename, "../assets/sprites/%d.png", i);
			numbertex[i] = LoadTexture(filename);
		}
	}
	birdtex[0] = LoadTexture("../assets/sprites/yellowbird-upflap.png");
	birdtex[1] = LoadTexture("../assets/sprites/yellowbird-midflap.png");
	birdtex[2] = LoadTexture("../assets/sprites/yellowbird-downflap.png");
	birdtex[3] = LoadTexture("../assets/sprites/bluebird-upflap.png");
	birdtex[4] = LoadTexture("../assets/sprites/bluebird-midflap.png");
	birdtex[5] = LoadTexture("../assets/sprites/bluebird-downflap.png");
	birdtex[6] = LoadTexture("../assets/sprites/redbird-upflap.png");
	birdtex[7] = LoadTexture("../assets/sprites/redbird-midflap.png");
	birdtex[8] = LoadTexture("../assets/sprites/redbird-downflap.png");

	// load sound effects
	Sound pointfx = LoadSound("../assets/audio/point.ogg");
	Sound swooshfx = LoadSound("../assets/audio/swoosh.ogg");
	wingfx = LoadSound("../assets/audio/wing.ogg");
	hitfx = LoadSound("../assets/audio/hit.ogg");

	// initialize entities
	Bird population[POPULATION];
	NeuralNetwork nn[POPULATION];
	for (int i = 0; i < POPULATION; i++) {
		population[i] = bird_new();
		nn_new(&nn[i]);
	}
	initialize_pipes();

	point_timer = (pipes[0].x - population[0].pos.x) / SCROLL_SPEED;
	int gen = 0;
	while (!WindowShouldClose()) {
		float ft = GetFrameTime();
		
		// bg movement
		static float bg_posx = 0.0f;
		bg_posx -= SCROLL_SPEED * ft * 0.5f;
		if (bg_posx < -SCREEN_WIDTH)
			bg_posx += SCREEN_WIDTH;
	
		// ground movement
		static float ground_posx = 0.0f;
		ground_posx -= SCROLL_SPEED * ft;
		if (ground_posx < -SCREEN_WIDTH)
			ground_posx += SCREEN_WIDTH;

		// move bird
		Vector2 p = next_pipe();
		for (int i = 0; i < POPULATION; i++) {
			if (!population[i].alive)
				continue;

			float nn_inputs[3] = {
				p.y / SCREEN_HEIGHT,
				population[i].pos.y / SCREEN_HEIGHT,
				fabsf(p.x - population[i].pos.x) / SCREEN_WIDTH
			};
			if (nn_feedforward(&nn[i], nn_inputs) > 0.0f)
				bird_flap(&population[i]);
			bird_update(ft, &population[i]);
		}

		// point timer
		point_timer -= ft;
		if (point_timer <= 0.0f) {
			point_timer += PIPE_SPREAD / SCROLL_SPEED;
			score++;
			PlaySound(pointfx);
		}

		// move pipes and check for bird-pipe collision
		bool all_dead = true;
		move_pipes(ft, &pipetex[bg_skin]);
		for (int i = 0; i < POPULATION; i++) {
			if (population[i].alive) {
				collide_pipes(&population[i], &pipetex[bg_skin]);
				all_dead = false;
			}
		}

		// prepare next generation
		if (all_dead) {
			// genetic crossover
			bird_sort(population, nn);
			nn_cross_multiple(nn, 24);
			printf("gen %d: %f\n", gen++, population[0].score);

			// reset
			for (int i = 0; i < POPULATION; i++)
				population[i] = bird_new();
			initialize_pipes();
			point_timer = (pipes[0].x - population[0].pos.x) / SCROLL_SPEED;
			score = 0;
			bg_skin = !bg_skin;
		}

		BeginDrawing();
		ClearBackground(BLACK);

		// draw background
		DrawTexture(bgtex[bg_skin], bg_posx, 0.0f, WHITE);
		DrawTexture(bgtex[bg_skin], bg_posx + bgtex[0].width, 0.0f, WHITE);

		// draw pipes
		draw_pipes(&pipetex[bg_skin]);

		// draw ground
		const float groundy = SCREEN_HEIGHT - GROUND_HEIGHT;
		DrawTexture(groundtex, ground_posx, groundy, WHITE);
		DrawTexture(groundtex, ground_posx + groundtex.width, groundy, WHITE);

		// draw bird and score
		for (int i = 0; i < POPULATION; i++)
			if (population[i].alive)
				bird_draw(&population[i]);
		if (population[0].alive)
			bird_draw(&population[0]);
		render_score(numbertex);

		// additional text about AI
		char help_text[64];
		sprintf(help_text, "Gen: %d", gen); 
		DrawText(help_text, 5, SCREEN_HEIGHT - 23, 20, BLACK);

		EndDrawing();
	}

	for (int i = 0; i < POPULATION; i++) {
		bird_free(&population[i]);
		nn_free(&nn[i]);
	}

	UnloadSound(pointfx);
	UnloadSound(swooshfx);
	UnloadSound(wingfx);
	UnloadSound(hitfx);

	UnloadTexture(bgtex[0]);
	UnloadTexture(bgtex[1]);
	UnloadTexture(pipetex[0]);
	UnloadTexture(pipetex[1]);
	UnloadTexture(groundtex);

	for (int i = 0; i < 9; i++)
		UnloadTexture(birdtex[i]);

	CloseAudioDevice();
	CloseWindow();
	return 0;
}
