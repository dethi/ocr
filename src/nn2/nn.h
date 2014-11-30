#ifndef _NN_H_
#define _NN_H_

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <assert.h>

struct net {
    size_t n_layer;
    struct layer *layers;
};

struct layer {
    size_t n_neuron;
    size_t w_per_neuron; // set to 0 if input layer
    double *w; // [n_neuron * w_per_neuron] not defined if input layer
    double *bias; // [n_neuron] not defined if input layer
    double *err; // [n_neuron * w_per_neuron] not defined if input layer
    double *out; // [n_neuron]
};

struct net net_init(size_t n_layer, size_t *n_neuron_per_layer);
void net_compute(struct net nwk, double *inputs);
struct net net_load(char *filename);
void net_save(struct net nwk, char *filename);
void net_free(struct net nwk);

void layer_init(struct layer *l, size_t n_neuron, size_t w_per_person);
void layer_calc_output(struct layer *l, double *inputs);
void layer_free(struct layer *l);

#endif
