#include "nn.h"

const double LEARNING = 0.4;
const double MOMENTUM = 0.1;

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
    return (1.0 - x) * x;
}

static
void cpyresult(double *dst, const double *src, size_t n, double th)
{
    //printf("brut out: ");
    for (size_t i = 0; i < n; ++i)
    {
        //printf("%f ", src[i]);
        dst[i] = (src[i] > th) ? 1 : 0;
    }
    //printf("\n");
}

struct net net_init(size_t n_layer, size_t *n_neuron_per_layer)
{
    assert(n_layer >= 2);
    struct layer *layers = malloc(sizeof(struct layer) * n_layer);
    assert(layers);

    srand(time(NULL));
    layer_init(&layers[0], n_neuron_per_layer[0], 0);
    for (size_t i = 1; i < n_layer; ++i)
        layer_init(&layers[i], n_neuron_per_layer[i], n_neuron_per_layer[i-1]);

    struct net network = { n_layer, layers };
    return network;
}

void net_train(struct net nwk, struct training t)
{
    unsigned epoch = 0;
    double error;

    double *results = malloc(sizeof(double) * t.n_out);
    assert(results);

    do {
        error = 0;

        for (size_t i = 0; i < t.n_set; ++i) {
            net_compute(nwk, get_in(&t, i));

            cpyresult(results, nwk.layers[nwk.n_layer - 1].out, t.n_out, 0.5);
            error += net_error(nwk, get_out(&t, i));

            printf("In: ");
            double *ptr = get_in(&t, i);
            for (size_t j = 0; j < t.n_in; ++j)
                printf("%.2f ", ptr[j]);
            printf("\nOut: ");
            for (size_t j = 0; j < t.n_out; ++j)
                printf("%.2f ", results[j]);
            printf("\nDesired: ");
            ptr = get_out(&t, i);
            for (size_t j = 0; j < t.n_out; j++)
                printf("%0.2f ", ptr[j]);
            printf("\n---\n");
        }

        error *= (1. / (double)t.n_set);
        printf("ERROR: %f\n", error);
        printf("******************************************\n");

        ++epoch;
    } while(error > 0.0001);

    printf("EPOCH: %d\n", epoch);
    free(results);
}

void net_compute(struct net nwk, double *inputs)
{
    for (size_t i = 0; i < nwk.layers[0].n_neuron; ++i)
        nwk.layers[0].out[i] = inputs[i];

    for (size_t i = 1; i < nwk.n_layer; ++i)
        layer_calc_output(&nwk.layers[i], nwk.layers[i-1].out);
}

double* net_output(struct net nwk)
{
    struct layer *l = &nwk.layers[nwk.n_layer-1];
    for (size_t i = 0; i < l->n_neuron; ++i)
        l->out[i] = (l->out[i] > 0.5) ? 1 : 0;
    return l->out;
}

double net_error(struct net nwk, double *desired)
{
    double global_error = 0;

    /* Output layer */
    struct layer *l_out = &nwk.layers[nwk.n_layer-1];
    for (size_t i = 0; i < l_out->n_neuron; ++i) {
        l_out->err[i] = df(l_out->out[i]) * (desired[i] - l_out->out[i]);
        global_error += pow(l_out->err[i], 2);
    }
    global_error *= 0.5;

    /* Foreach layer, output to input (exclude) */
    for (size_t layer = nwk.n_layer - 2; layer > 0; --layer) {
        struct layer *cur = &nwk.layers[layer];
        struct layer *next = &nwk.layers[layer+1];

        /* Foreach neuron in the current layer */
        for (size_t c = 0; c < cur->n_neuron; ++c) {
            double sum = 0;

            /* Foreach neuron in the next layer */
            for (size_t n = 0; n < next->n_neuron; ++n)
                sum += next->err[n] * next->w[get_w(next, n, c)];

            cur->err[c] = df(cur->out[c]) * sum;
        }
    }

    for (size_t layer = 1; layer < nwk.n_layer; ++layer) {
        struct layer *cur = &nwk.layers[layer];
        struct layer *prev = &nwk.layers[layer-1];

        for (size_t c = 0; c < cur->n_neuron; ++c) {
            cur->bias[c] += LEARNING * cur->err[c];

            for (size_t p = 0; p < prev->n_neuron; ++p) {
                size_t index = get_w(cur, c, p);
                double dw = LEARNING * cur->err[c] * prev->out[p];
                cur->w[index] += dw + MOMENTUM * cur->prev_dw[index];
                cur->prev_dw[index] = dw;
            }
        }
    }

    return global_error;
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
            l->prev_dw = calloc(sizeof(double), l->n_neuron * l->w_per_neuron);
            assert(l->prev_dw);
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
            l->prev_dw = NULL;
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
        l->prev_dw = calloc(sizeof(double), n_neuron * w_per_neuron);
        assert(l->prev_dw);
        l->bias = malloc(sizeof(double) * n_neuron);
        assert(l->bias);
        l->err = calloc(sizeof(double), n_neuron);
        assert(l->err);

        for (size_t i = 0; i < n_neuron;  ++i) {
            l->bias[i] = -1. + 2. * ((double)rand() / RAND_MAX);
            //printf("bias: %f\tw:", l->bias[i]);

            for (size_t j = 0; j < w_per_neuron; ++j) {
                l->w[get_w(l, i, j)] = -1. + 2. * ((double)rand() / RAND_MAX);
                //printf("%f ", l->w[get_w(l,i,j)]);
            }
            //printf("\n");
        }
    } else {
        l->w = NULL;
        l->prev_dw = NULL;
        l->bias = NULL;
        l->err = NULL;
    }

    l->out = malloc(sizeof(double) * n_neuron);
    assert(l->out);
}

void layer_calc_output(struct layer *l, double *inputs)
{
    for (size_t i = 0; i < l->n_neuron; ++i) {
        double sigma = 0;
        for (size_t j = 0; j < l->w_per_neuron; ++j)
            sigma += l->w[get_w(l, i, j)] * inputs[j];
        l->out[i] = f(sigma + l->bias[i]);
    }
}

void layer_free(struct layer *l)
{
    free(l->w);
    free(l->prev_dw);
    free(l->bias);
    free(l->err);
    free(l->out);
}

void training_free(struct training t)
{
    free(t.in);
    free(t.out);
    t.n_set = 0;
    t.n_in = 0;
    t.n_out = 0;
}
