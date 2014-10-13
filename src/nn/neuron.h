#ifndef _NEURON_H_
#define _NEURON_H_

#include "connection.h"

typedef struct neuron {
    float output;
    struct conn links[];
    size_t nlinks;
    int bias; //BOOL

    void (*calc_out) (struct neuron*);
    void (*add_link) (struct neuron*, struct conn*);
    void (*free) (struct neuron*);
} neuron;

neuron* new_neuron(int bias);
void neuron_calc_out(neuron *this);
void neuron_add_link(neuron *this, conn *l);
void neuron_free(neuron *this);

#endif
