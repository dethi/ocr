#include "neuron.h"

static void neuron_init(neuron *this, int bias);
static double f(double x);

neuron* new_neuron(int bias)
{
    neuron *this = malloc(sizeof(neuron));
    if (!this)
        return NULL;

    neuron_init(this, bias);
    return this;
}

static void neuron_init(neuron *this, int bias)
{
    this->bias = bias;
    this->out = (bias) ? 1 : 0;
    this->nlink = 0;
    this->links = NULL;
}

void neuron_calc_out(neuron *this)
{
    if (this->bias)
        return;

    double sum = 0;
    double bias = 0;

    for (size_t i = 0; i < this->nlink; ++i) {
        conn *l = this->links[i];
        neuron *from = l->from;
        neuron *to = l->to;

        if (to == this) {
            if (from->bias)
                bias = from->out * l->weight;
            else
                sum += from->out * l->weight;
        }

        this->out = f(bias + sum);
    }
}

static double f(double x)
{
    return 1.0 / (1.0 + exp(-x));
}

void neuron_add_link(neuron *this, conn *l)
{
    conn **ptr = realloc(this->links, sizeof(conn *) * ++(this->nlink));
    if (!ptr)
        exit(EXIT_FAILURE);

    this->links = ptr;
    this->links[this->nlink - 1] = l;
}
