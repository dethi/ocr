#ifndef _CONNECTION_H_
#define _CONNECTION_H_

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "neuron.h"

typedef struct conn {
    struct neuron *from;
    struct neuron *to;
    double weight;
    double dlast;
} conn;

conn* new_conn(struct neuron *from, struct neuron *to);
void conn_adjust_weight(conn *this, double dw);

#endif
