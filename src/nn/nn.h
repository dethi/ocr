#ifndef _NN_H_
#define _NN_H_

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <assert.h>
#include <string.h>

struct net {
    size_t n_layer;
    struct layer *layers;
};

struct layer {
    size_t n_neuron;
    size_t w_per_neuron; // set to 0 if input layer
    double *w; // [n_neuron * w_per_neuron] not defined if input layer
    double *prev_dw; // [n_neuron * w_per_neuron] not defined if input layer
    double *bias; // [n_neuron] not defined if input layer
    double *err; // [n_neuron] not defined if input layer
    double *out; // [n_neuron]
};

struct training {
    double *in;
    double *out;
    size_t n_set;
    size_t n_in;
    size_t n_out;
};

static inline
double* get_in(struct training *t, size_t i)
{
    return &t->in[i * t->n_in];
}

static inline
double* get_out(struct training *t, size_t i)
{
    return &t->out[i * t->n_out];
}

struct net net_init(size_t n_layer, size_t *n_neuron_per_layer);
void net_train(struct net nwk, struct training t);
void net_compute(struct net nwk, double *inputs);
double *net_output(struct net nwk);
double net_error(struct net nwk, double *desired);
struct net net_load(char *filename);
void net_save(struct net nwk, char *filename);
void net_free(struct net nwk);

void layer_init(struct layer *l, size_t n_neuron, size_t w_per_person);
void layer_calc_output(struct layer *l, double *inputs);
void layer_free(struct layer *l);

void training_free(struct training t);

#endif
