#include "network.h"

const double LEARNING = 0.3;

static void net_init(net *this, size_t ninput, size_t nhidden);

net* new_net(size_t ninput, size_t nhidden)
{
    net *this = malloc(sizeof(net));
    if (!this)
        return NULL;

    net_init(this, ninput, nhidden);
    return this;
}

static void net_init(net *this, size_t ninput, size_t nhidden)
{
    this->inputs = malloc(sizeof(neuron *) * (ninput + 1));
    this->hiddens = malloc(sizeof(neuron *) * (nhidden + 1));

    // Make inputs and hiddens neurons
    for (size_t i = 0; i < ninput; ++i)
        this->inputs[i] = new_neuron(0);
    for (size_t i = 0; i < nhidden; ++i)
        this->hiddens[i] = new_neuron(0);

    // Make bias neurons
    this->inputs[ninput] = new_neuron(1);
    this->hiddens[nhidden] = new_neuron(1);

    // Make output neuron
    this->output = new_neuron(0);

    this->ninput = ninput + 1;
    this->nhidden = nhidden + 1;

    // Connect input layer to hidden layer
    for (size_t i = 0; i < this->ninput; ++i) {
        for (size_t j = 0; j < this->nhidden - 1; ++j) {
            conn *c = new_conn(this->inputs[i], this->hiddens[j]);
            neuron_add_link(this->inputs[i], c);
            neuron_add_link(this->hiddens[j], c);
        }
    }

    // Connect hidden layer to output
    for (size_t i = 0; i < this->nhidden; ++i) {
        conn *c = new_conn(this->hiddens[i], this->output);
        neuron_add_link(this->hiddens[i], c);
        neuron_add_link(this->output, c);
    }
}

double net_feedforward(net *this, double *values, size_t n)
{
    for (size_t i = 0; i < n; ++i)
        this->inputs[i]->out = values[i];

    for (size_t i = 0; i < this->nhidden - 1; ++i)
        neuron_calc_out(this->hiddens[i]);

    neuron_calc_out(this->output);
    return this->output->out;
}

double net_train(net *this, double *values, size_t n, double answer)
{
    double output = net_feedforward(this, values, n);
    double derror = answer - output;
    double dydx_out = output * (1 - output);

    for (size_t i = 0; i < this->nhidden; ++i) {
        neuron *hide = this->hiddens[i];
        double derror_hide = 0;

        for (size_t j = 0; j < hide->nlink; ++j) {
            conn *c = hide->links[j];

            if (c->from == hide)
                derror_hide += c->weight * derror;
        }

        for (size_t j = 0; j < hide->nlink; ++j) {
            conn *c = hide->links[j];

            if (c->to == hide) {
                double dydx = hide->out * (1 - hide->out);
                double d = LEARNING * derror_hide * dydx * c->from->out;
                conn_adjust_weight(c, d);
            }
        }

        for (size_t j = 0; j < hide->nlink; ++j) {
            conn *c = hide->links[j];

            if (c->from == hide) {
                double d = LEARNING * derror * dydx_out * hide->out;
                conn_adjust_weight(c, d);
            }
        }
    }

    return output;
}
