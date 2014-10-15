#include "neuron.h"

static void neuron_init(neuron *this);
static float f(float x);

neuron* new_neuron(int bias)
{
    neuron *this = malloc(sizeof(neuron));
    if (!this)
        return NULL;

    neuron_init(this);
    if (bias) {
        this->output = bias;
        this->bias = 1;
    }

    return this;
}

static void neuron_init(neuron *this)
{
    this->output = 0;
    this->links = NULL;
    this->nlinks = 0;
    this->bias = 0;

    this->calc_out = neuron_calc_out;
    this->add_link = neuron_add_link;
    this->free = neuron_free;
}

void neuron_calc_out(neuron *this)
{
    if (this->bias)
        return;

    float sum = 0;
    float bias = 0;

    for (size_t i = 0; i < this->nlinks; ++i) {
        conn *l = &this->links[i];
        neuron *from = l->from;
        neuron *to = l->to;

        if (to == this) {
            if (from->bias)
                bias = from->output * l->weight;
            else
                sum += from->output * l->weight;
        }

        this->output = f(bias + sum);
    }
}

static float f(float x)
{
    return 1.0f / (1.0f + (float)exp(-x));
}

void neuron_add_link(neuron *this, conn *l)
{
    conn *ptr = realloc(this->links, sizeof(conn) * (++this->nlinks));
    if (!ptr) {
        this->free(this);
        exit(EXIT_FAILURE);
    }

    this->links = ptr;
    this->links[this->nlinks] = *l;
}

void neuron_free(neuron *this)
{
    free(this->links);
    free(this);
}
