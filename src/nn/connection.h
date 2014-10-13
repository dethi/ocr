#ifndef _CONNECTION_H_
#define _CONNECTION_H_

#include "neuron.h"

typedef struct conn {
    struct neuron *from;
    struct neuron *to;
    float weight;

    void (*adjust_weight) (struct conn*, float);
    void (*free) (struct conn*)
} conn;

conn* new_conn(neuron *from, *neuron to);
void conn_adjust_weight(conn *this, float dw);
void conn_free (conn *this);

#endif
