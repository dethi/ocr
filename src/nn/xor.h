#ifndef _XOR_H_
#define _XOR_H_

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

struct neuron {
    double error;
    double bias;
    double in[2];
    double w[2];
};

struct net {
    struct neuron hidden1;
    struct neuron hidden2;
    struct neuron output;
};

void net_main();
void net_train(struct net *this);
void net_randomize_w(struct net *this);
double net_calc_output(struct net *this, double in1, double in2);
double neuron_calc_output(struct neuron *this);
void neuron_randomize_w(struct neuron *this);
void neuron_adjust_w(struct neuron *this);

#endif
