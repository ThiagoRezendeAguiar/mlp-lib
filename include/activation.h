#ifndef ACTIVATION_H
#define ACTIVATION_H

#include <math.h>

typedef struct {
    double (*func)(double);
    double (*derivative)(double);
} Activation;

double sigmoid(double x);
double sigmoid_derivative(double x);

#endif