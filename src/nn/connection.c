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
    this->weight = -1.0 * 2.0 * ((double)rand() / (double)RAND_MAX);
    this->dlast = 0;
}

void conn_adjust_weight(conn *this, double dw)
{
    this->weight += dw + 0.8 * this->dlast;
    this->dlast = dw;
}
