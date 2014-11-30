#ifndef _NN_H_
#define _NN_H_

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <assert.h>

typedef struct layer {
    size_t n_neuron;
    size_t w_per_neuron; // set to 0 if input layer
    double *w; // not defined if input layer
    double *bias; // not defined if input layer;
    double *out;
    double *err; // not defined if input layer
} layer;

layer* net_init(size_t n_layer, size_t *n_neuron_per_layer);
void net_set_input(layer *network, double *inputs);

void layer_init(layer *l, size_t n_neuron, size_t w_per_person);
void layer_calc_output(layer *l, double *inputs);

#endif
