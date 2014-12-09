struct t_neuron {
    int bias;
    double output;
    size_t nlink;
    struct t_connection **links;
};

struct t_connection {
    struct t_neuron *from;
    struct t_neuron *to;
    double weight;
};

struct t_network {
    size_t nb_input;
    size_t nb_layer;
    size_t nb_hidden;
    size_t nb_output;
    struct t_neuron **inputs;
    struct t_neuron **hiddens;
    struct t_neuron **output;
};