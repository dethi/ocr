#ifndef _NEURON_H_
#define _NEURON_H_

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "connection.h"

typedef struct neuron {
    int bias; //BOOL
    double out;
    size_t nlink;
    struct conn **links;
} neuron;

neuron* new_neuron(int bias);
void neuron_calc_out(neuron *this);
void neuron_add_link(neuron *this, struct conn *l);

#endif
