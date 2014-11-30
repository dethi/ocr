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
    assert(n_layer >= 2);
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

void net_error(struct net nwk, double *desired)
{
    /* Foreach output neuron */
    struct layer *l = &nwk.layers[nwk.n_layer - 1];
    for (size_t i = 0; i < l->n_neuron; ++i) {
        /* Compute global error */
        l->err[i] = df(l->out[i]) * (desired[i] - f(l->out[i]));
    }

    /* Foreach layers, output to input (exclude) */
    for (size_t j = nwk.n_layer - 1; j > 1; --j) {
        struct layer *cur_l = &nwk.layers[j-1];
        struct layer *prev_l = &nwk.layers[j];

        /* Foreach neuron in current layer */
        for (size_t k = 0; k < cur_l->n_neuron; ++k) {
            /* Compute propaged error */
            double error = 0;
            for (size_t h = 0; h < prev_l->n_neuron; ++h)
                error += prev_l->w[get_w(prev_l, h, k)] * prev_l->err[h];

            /* Compute current error */
            cur_l->err[k] = df(cur_l->out[k]) * error;
        }
    }

    /* Apply error */
    for (size_t i = 1; i < nwk.n_layer; ++i) {
        struct layer *l = &nwk.layers[i];

        for (size_t j = 0; j < l->n_neuron; ++j) {
            l->bias[j] += l->err[j];
            for (size_t k = 0; k < l->w_per_neuron; ++k)
                l->w[get_w(l, j, k)] += l->err[j] * nwk.layers[i-1].out[k];
        }
    }
}

struct net net_load(char *filename)
{
    assert(filename);

    FILE *file = fopen(filename, "r");
    assert(file);

    char magic_code[10];
    fscanf(file, "%s", magic_code);
    assert(strcmp(magic_code, "--nn--") == 0);

    struct net nwk;
    fscanf(file, "%lu", &nwk.n_layer);
    nwk.layers = malloc(sizeof(struct layer) * nwk.n_layer);
    assert(nwk.layers);

    for (size_t i = 0; i < nwk.n_layer; ++i) {
        struct layer *l = &nwk.layers[i];
        fscanf(file, "%lu %lu", &l->n_neuron, &l->w_per_neuron);

        if (l->w_per_neuron) {
            l->w = malloc(sizeof(double) * l->n_neuron * l->w_per_neuron);
            assert(l->w);
            l->bias = malloc(sizeof(double) * l->n_neuron);
            assert(l->bias);
            l->err = calloc(sizeof(double), l->n_neuron);
            assert(l->err);

            for (size_t j = 0; j < l->n_neuron; ++j) {
                for (size_t k = 0; k < l->w_per_neuron; ++k)
                    fscanf(file, "%la", &l->w[get_w(l, j, k)]);
            }

            for (size_t j = 0; j < l->n_neuron; ++j)
                fscanf(file, "%la", &l->bias[j]);
        } else {
            l->w = NULL;
            l->bias = NULL;
            l->err = NULL;
        }

        l->out = malloc(sizeof(double) * l->n_neuron);
        assert(l->out);
    }

    return nwk;
}

void net_save(struct net nwk, char *filename)
{
    assert(filename);

    FILE *file = fopen(filename, "w");
    assert(file);

    fprintf(file, "--nn--\n");
    fprintf(file, "%lu\n", nwk.n_layer);

    for (size_t i = 0; i < nwk.n_layer; ++i) {
        struct layer *l = &nwk.layers[i];
        fprintf(file, "%lu %lu\n", l->n_neuron, l->w_per_neuron);

        if (l->w_per_neuron) {
            for (size_t j = 0; j < l->n_neuron; ++j) {
                for (size_t k = 0; k < l->w_per_neuron; ++k)
                    fprintf(file, "%a\n", l->w[get_w(l, j, k)]);
            }

            for (size_t j = 0; j < l->n_neuron; ++j)
                fprintf(file, "%a\n", l->bias[j]);
        }
    }

    fclose(file);
}

void net_free(struct net nwk)
{
    nwk.n_layer = 0;
    free(nwk.layers);
}

void layer_init(struct layer *l, size_t n_neuron, size_t w_per_neuron)
{
    l->n_neuron = n_neuron;
    l->w_per_neuron = w_per_neuron;

    if (w_per_neuron) {
        l->w = malloc(sizeof(double) * n_neuron * w_per_neuron);
        assert(l->w);

        l->bias = malloc(sizeof(double) * n_neuron);
        assert(l->bias);

        for (size_t i = 0; i < n_neuron;  ++i) {
            srand(time(NULL));
            l->bias[i] = ((double)rand() / (double)RAND_MAX);

            for (size_t j = 0; j < w_per_neuron; ++j) {
                l->w[get_w(l, i, j)] = ((double)rand() / (double)RAND_MAX);
            }
        }

        l->err = calloc(sizeof(double), n_neuron);
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
            out += l->w[get_w(l, i, j)] * f(inputs[j]);
        l->out[i] = out + f(l->bias[i]);
    }
}

void layer_free(struct layer *l)
{
    free(l->w);
    free(l->bias);
    free(l->err);
    free(l->out);
}
