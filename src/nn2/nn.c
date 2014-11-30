#include "nn.h"

static inline
size_t get_w(layer *l, size_t i_neuron, size_t i_w)
{
    return i_w + i_neuron * l->w_per_neuron;
}

static inline
double f(double x)
{
    return 1.0 / (1.0 + exp(-x));
}

static inline
double df(double x)
{
    return x * (1.0 - x);
}

layer* net_init(size_t n_layer, size_t *n_neuron_per_layer)
{
    assert(n_layer);
    layer *network = malloc(sizeof(layer) * n_layer);
    assert(network);

    layer_init(&network[0], n_neuron_per_layer[0], 0);
    for (size_t i = 1; i < n_layer; ++i)
        layer_init(&network[i], n_neuron_per_layer[i], n_neuron_per_layer[i-1]);

    return network;
}

void net_set_input(layer *network, double *inputs)
{
    for (size_t i = 0; i < network[0].n_neuron; ++i)
        network[0].out[i] = inputs[i];
}

void layer_init(layer *l, size_t n_neuron, size_t w_per_neuron)
{
    l->n_neuron = n_neuron;
    l->w_per_neuron = w_per_neuron;

    l->out = malloc(sizeof(double) * n_neuron);
    assert(l->out);

    if (w_per_neuron) {
        l->w = malloc(sizeof(double) * w_per_neuron * n_neuron);
        assert(l->w);

        l->bias = malloc(sizeof(double) * n_neuron);
        assert(l->bias);

        for (size_t i = 0; i < n_neuron;  ++i) {
            srand(time(NULL));
            l->bias[i] = -1.0 * 2.0 * ((double)rand() / (double)RAND_MAX);

            for (size_t j = 0; j < w_per_neuron; ++j) {
                l->w[get_w(l, i, j)] = -1.0 + 2.0 *
                    ((double)rand() / (double)RAND_MAX);
            }
        }

        l->err = calloc(sizeof(double), w_per_neuron * n_neuron);
        assert(l->err);
    }
}

void layer_calc_output(layer *l, double *inputs)
{
    for (size_t i = 0; i < l->n_neuron; ++i) {
        double out = 0;
        for (size_t j = 0; j < l->w_per_neuron; ++j)
            out += l->w[get_w(l, i, j)] * inputs[j];
        l->out[i] = out + l->bias[i];
    }
}

