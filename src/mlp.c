#include "mlp.h"

// num_layers deve ser igual a length de layer_sizes
MLP *mlp_create(int num_layers, const int *layer_sizes) {
    if (num_layers < 2 || layer_sizes == NULL) {
        fprintf(stderr, "Erro: A rede deve ter pelo menos 2 camadas (entrada e saída).\n");
        return NULL;
    }

    MLP *net = (MLP *)malloc(sizeof(MLP));

    if (net == NULL) return NULL;

    net->num_layers = num_layers;
    net->layers_sizes = (int *)malloc(num_layers * sizeof(int));

    if (net->layers_sizes == NULL) {
        free(net);
        return NULL;
    }

    memcpy(net->layers_sizes, layer_sizes, num_layers * sizeof(int));

    int num_processing_layers = num_layers - 1;
    net->layers = (Layer *)malloc(num_processing_layers * sizeof(Layer));

    if (net->layers == NULL) {
        free(net->layers_sizes);
        free(net);
        return NULL;
    }

    printf("Aqui\n");

    for (int i = 0; i < num_processing_layers; i++) {
        int num_inputs = layer_sizes[i];
        int num_outputs = layer_sizes[i + 1];

        net->layers[i].num_inputs = num_inputs;
        net->layers[i].num_outputs = num_outputs;

        net->layers[i].weights = matrix_create(num_outputs, num_inputs);
        net->layers[i].biases = matrix_create(num_outputs, 1);
        // net->layers[i].outputs = matrix_create(num_outputs, 1);
        // net->layers[i].delta = matrix_create(num_outputs, 1);

        net->layers[i].activation.func = sigmoid;
        net->layers[i].activation.derivative = sigmoid_derivative;

        if (net->layers[i].weights == NULL || net->layers[i].biases == NULL ) {
            // || net->layers[i].outputs == NULL || net->layers[i].delta == NULL) {
           
            matrix_free(net->layers[i].weights);
            matrix_free(net->layers[i].biases);
            // matrix_free(net->layers[i].outputs);
            // matrix_free(net->layers[i].delta);

            for (int j = 0; j < i; j++) {
                matrix_free(net->layers[j].weights);
                matrix_free(net->layers[j].biases);
                // matrix_free(net->layers[j].outputs);
                // matrix_free(net->layers[j].delta);
            }

            free(net->layers);
            free(net->layers_sizes);
            free(net);
            return NULL;
        }

        matrix_randomize(net->layers[i].weights, i);
        matrix_randomize(net->layers[i].biases, i + 1000);
    }

    return net;
}

void mlp_free(MLP *net) {
    if (net == NULL) return;

    int num_processing_layers = net->num_layers - 1;
    if (net->layers != NULL) {
        for (int i = 0; i < num_processing_layers; i++) {
            matrix_free(net->layers[i].weights);
            matrix_free(net->layers[i].biases);
            matrix_free(net->layers[i].outputs);
            matrix_free(net->layers[i].delta);
        }

        free(net->layers);
    }
    free(net->layers_sizes);
    free(net);
}

Matrix* mlp_feedforward(MLP* net, const Matrix* inputs) {
    Matrix* current_outputs = matrix_copy(inputs);

    int num_processing_layers = net->num_layers - 1;
    for (int i = 0;i < num_processing_layers; i++) {
        matrix_free(net->layers[i].net_inputs);

        Matrix* weighted_sum = matrix_multiply(net->layers[i].weights, current_outputs);
        net->layers[i].net_inputs = matrix_add(weighted_sum, net->layers[i].biases);

        matrix_free(weighted_sum);
        matrix_free(net->layers[i].outputs);

        net->layers[i].outputs = matrix_apply_function(net->layers[i].net_inputs, net->layers[i].activation.func);
        
        matrix_free(current_outputs);
        current_outputs = matrix_copy(net->layers[i].outputs);
    }

    return current_outputs;
}

void mlp_backpropagate(MLP* net, const Matrix* targets) {
    int num_processing_layers = net->num_layers - 1; 

    Layer* output_layer = &net->layers[num_processing_layers - 1];
    Matrix* output_error = matrix_subtract(targets, output_layer->outputs);
    Matrix* gradients = matrix_apply_function(output_layer->outputs, output_layer->activation.derivative);
    Matrix* output_delta = matrix_elementwise_multiply(output_error, gradients);
    matrix_free(output_layer->delta);
    output_layer->delta = output_delta;
    matrix_free(output_error);
    matrix_free(gradients);

    for (int i = num_processing_layers - 2; i >= 0; i--) {
        Layer* hidden_layer = &net->layers[i];
        Layer* next_layer = &net->layers[i + 1];
        Matrix* weights_t = matrix_transpose(next_layer->weights);
        Matrix* propagated_error = matrix_multiply(weights_t, next_layer->delta);
        Matrix* hidden_gradients = matrix_apply_function(hidden_layer->outputs, hidden_layer->activation.derivative);
        Matrix* hidden_delta = matrix_elementwise_multiply(propagated_error, hidden_gradients);
        matrix_free(hidden_layer->delta);
        hidden_layer->delta = hidden_delta;
        matrix_free(weights_t);
        matrix_free(propagated_error);
        matrix_free(hidden_gradients);
    }
}

void mlp_update_weights(MLP* net, const Matrix* inputs, double learning_rate) {
    int num_processing_layers = net->num_layers - 1; 

    Matrix* current_inputs = matrix_copy(inputs);
    for (int i = 0; i < num_processing_layers; i++) {
        Layer* layer = &net->layers[i];

        Matrix* inputs_t = matrix_transpose(current_inputs);
        Matrix* weight_deltas = matrix_multiply(layer->delta, inputs_t);
        Matrix* scaled_Weight_deltas = matrix_scale(weight_deltas, learning_rate);
        Matrix* new_weights = matrix_add(layer->weights, scaled_Weight_deltas);
        matrix_free(layer->weights);
        layer->weights = new_weights;

        Matrix* scaled_bias_deltas = matrix_scale(layer->delta, learning_rate);
        Matrix* new_biases = matrix_add(layer->biases, scaled_bias_deltas);
        matrix_free(layer->biases);
        layer->biases = new_biases;

        matrix_free(inputs_t);
        matrix_free(weight_deltas);
        matrix_free(scaled_Weight_deltas);
        matrix_free(scaled_bias_deltas);

        matrix_free(current_inputs);
        current_inputs = matrix_copy(layer->outputs);
    }
    matrix_free(current_inputs);
}

void mlp_train(MLP* net, const Matrix* inputs, const Matrix* targets,double learning_rate) {
    Matrix* predictions = mlp_feedforward(net, inputs);
    matrix_free(predictions);
    mlp_backpropagate(net, targets);
    mlp_update_weights(net, inputs, learning_rate);
}