#ifndef _NETWORK_H_
#define _NETWORK_H_

#include <stdlib.h>
#include <stdio.h>
#include "neuron.h"
#include "connection.h"

typedef struct net {
    size_t ninput;
    size_t nhidden;
    struct neuron *output;
    struct neuron **inputs;
    struct neuron **hiddens;
} net;

net* new_net(size_t ninput, size_t nhidden);
float net_feedforward(net *this, float *values, size_t n);
float net_train(net *this, float *values, size_t n, float answer);

#endif
