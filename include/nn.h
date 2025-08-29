#ifndef __nn_h__
#define __nn_h__

// brain shape will be 3x2x2x2
#define NN_LAYERS 4
#define WEIGHT_COUNT (3 * 2 + 2 * 2 + 2 * 2)
#define BIAS_COUNT (2 + 2 + 2)

typedef struct {
	float weight[WEIGHT_COUNT];
	float bias[BIAS_COUNT];
} NeuralNetwork;

void nn_new(NeuralNetwork* nn);
void nn_free(NeuralNetwork* nn);
float nn_feedforward(NeuralNetwork* nn, float inputs[3]);
void nn_cross(NeuralNetwork* res, NeuralNetwork* a, NeuralNetwork* b);

void nn_cross_multiple(NeuralNetwork* nns, int best_cutoff);

#endif
