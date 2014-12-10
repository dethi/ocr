struct net {
    size_t n_layer;
    struct layer *layers;
};

struct layer {
    size_t n_neuron;
    size_t w_per_neuron;
    double *w;
    double *prev_dw;
    double *err;
    double *out;
};