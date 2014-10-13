#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "connection.h"

static void conn_init(conn *this, neuron *from, neuron *to);

conn* new_conn(neuron *from, neuron *to)
{
    conn *this = malloc(sizeof(conn));
    if (!this)
        return NULL;

    conn_init(this, from, to);
    return this;
}

static void conn_init(conn *this, neuron *from, neuron *to)
{
    srand(time(NULL));

    this->from = from;
    this->to = to;
    this->weight = -1 * 2 * ((float)rand() / RAND_MAX);

    this->adjust_weight = conn_adjust_weight;
    this->free = conn_free;
}

void conn_adjust_weight(conn *this, float dw)
{
    this->weight += dw;
}

void conn_free(conn *this)
{
    free(this);
}
