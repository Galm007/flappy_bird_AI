#include "nn.h"
#include "constants.h"
#include "raylib.h"

#include <string.h>

#define RANDF() (GetRandomValue(0, 1000000) / 1000000.0f)

void nn_new(NeuralNetwork* nn) {
	for (int i = 0; i < WEIGHT_COUNT; i++)
		nn->weight[i] = RANDF();
	for (int i = 0; i < BIAS_COUNT; i++)
		nn->bias[i] = RANDF() * 2.0f - 1.0f;
}

void nn_free(NeuralNetwork* nn) {
}

float nn_feedforward(NeuralNetwork* nn, float inputs[3]) {
	int nn_shape[NN_LAYERS] = { 3, 2, 2, 2 };

	float buffer[2][3];
	memset(buffer, 0.0f, 6 * sizeof(float));
	for (int i = 0; i < 3; i++)
		buffer[0][i] = inputs[i];

	int wshift = 0, bshift = 0;
	for (int i = 1; i < 4; i++) {
		for (int j = 0; j < nn_shape[i]; j++) {
			for (int k = 0; k < nn_shape[i - 1]; k++) {
				buffer[i % 2][j] += buffer[(i - 1) % 2][k] * nn->weight[wshift++] + nn->bias[bshift];
			}
		}
		bshift++;
	}

	return buffer[1][0] - buffer[1][1];
}

void nn_cross(NeuralNetwork* dest, NeuralNetwork* a, NeuralNetwork* b) {
	// cross weights
	int cp = GetRandomValue(1, WEIGHT_COUNT - 1);
	memcpy(dest->weight, a->weight, cp * sizeof(float));
	memcpy(&dest->weight[cp], &b->weight[cp], (WEIGHT_COUNT - cp) * sizeof(float));

	// cross biases
	cp = GetRandomValue(1, BIAS_COUNT - 1);
	memcpy(dest->bias, a->bias, cp * sizeof(float));
	memcpy(&dest->bias[cp], &b->bias[cp], (BIAS_COUNT - cp) * sizeof(float));

	// mutate weights
	for (int i = 0; i < WEIGHT_COUNT; i++)
		if (RANDF() < MUTATION_THRESHOLD)
			dest->weight[i] = RANDF();

	// mutate biases
	for (int i = 0; i < BIAS_COUNT; i++)
		if (RANDF() < MUTATION_THRESHOLD)
			dest->bias[i] = RANDF() * 2.0f - 1.0f;
}

void nn_cross_multiple(NeuralNetwork* nn_arr, int best_cutoff) {
	int c = 0;
	for (int i = 0; i < best_cutoff; i++) {
		for (int j = 0; j < best_cutoff - i - 1; j++) {
			nn_cross(&nn_arr[best_cutoff + c], &nn_arr[i], &nn_arr[i + j + 1]);
			if (++c >= POPULATION - best_cutoff)
				return;
		}
	}
}
