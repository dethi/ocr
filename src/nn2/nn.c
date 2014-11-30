#include "nn.h"

static inline
size_t get_w(struct layer *l, size_t i_neuron, size_t i_w)
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

struct net net_init(size_t n_layer, size_t *n_neuron_per_layer)
{
    assert(n_layer);
    struct layer *layers = malloc(sizeof(struct layer) * n_layer);
    assert(layers);

    layer_init(&layers[0], n_neuron_per_layer[0], 0);
    for (size_t i = 1; i < n_layer; ++i)
        layer_init(&layers[i], n_neuron_per_layer[i], n_neuron_per_layer[i-1]);

    struct net network = { n_layer, layers };
    return network;
}

void net_compute(struct net nwk, double *inputs)
{
    for (size_t i = 0; i < nwk.layers[0].n_neuron; ++i)
        nwk.layers[0].out[i] = inputs[i];

    for (size_t i = 1; i < nwk.n_layer; ++i)
        layer_calc_output(&nwk.layers[i], nwk.layers[i-1].out);
}

void net_free(struct net nwk)
{
    free(nwk.layers);
}

void layer_init(struct layer *l, size_t n_neuron, size_t w_per_neuron)
{
    l->n_neuron = n_neuron;
    l->w_per_neuron = w_per_neuron;

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
    } else {
        l->w = NULL;
        l->bias = NULL;
        l->err = NULL;
    }

    l->out = malloc(sizeof(double) * n_neuron);
    assert(l->out);
}

void layer_calc_output(struct layer *l, double *inputs)
{
    for (size_t i = 0; i < l->n_neuron; ++i) {
        double out = 0;
        for (size_t j = 0; j < l->w_per_neuron; ++j)
            out += l->w[get_w(l, i, j)] * inputs[j];
        l->out[i] = out + l->bias[i];
    }
}

void layer_free(struct layer *l)
{
    free(l->w);
    free(l->bias);
    free(l->err);
    free(l->out);
}
