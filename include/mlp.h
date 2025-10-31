#include "matrix.h"
#include "activation.h"

#ifndef MLP_H
#define MLP_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int num_inputs;
    int num_outputs;

    Matrix* weights;
    Vector* biases;

    Activation activation;

    Vector* net_inputs;
    Vector* outputs;
    Vector* delta;
} Layer;

typedef struct {
    int num_layers;
    Layer* layers;
    int* layers_sizes;
} MLP;

MLP* mlp_create(int num_layers, const int* layer_sizes);

void mlp_free(MLP* net);

Matrix* mlp_feedforward(MLP* net, const Matrix* inputs);

void mlp_train(MLP* net, const Matrix* inputs, const Matrix* targets, double learning_rate);

#endif 