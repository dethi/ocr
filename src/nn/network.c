#include "network.h"

const float LEARNING = 0.5;

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

float net_feedforward(net *this, float *values, size_t n)
{
    for (size_t i = 0; i < n; ++i)
        this->inputs[i]->out = values[i];

    for (size_t i = 0; i < this->nhidden - 1; ++i)
        neuron_calc_out(this->hiddens[i]);

    neuron_calc_out(this->output);
    return this->output->out;
}

float net_train(net *this, float *values, size_t n, float answer)
{
    float result = net_feedforward(this, values, n);
    float d_output = result * (1 - result) * (answer - result);

    // Backpropagation
    // Apply delta to link between hidden and output
    for (size_t i = 0; i < this->output->nlink; ++i) {
        conn *c = this->output->links[i];
        neuron *from = c->from;
        float output = from->out;
        float d_weight = output * d_output;
        conn_adjust_weight(c, LEARNING * d_weight);
    }

    // Adjust hidden weights
    for (size_t i = 0; this->nhidden; ++i) {
        float sum = 0;

        for (size_t j = 0; j < this->hiddens[i]->nlink; ++j) {
            conn *c = this->hiddens[i]->links[j];

            if (c->from == this->hiddens[i])
                sum += c->weight * d_output;
        }

        for (size_t j = 0; j < this->hiddens[i]->nlink; ++j) {
            conn *c = this->hiddens[i]->links[j];

            if (c->to == this->hiddens[i]) {
                float output = this->hiddens[i]->out;
                float d_hidden = output * (1 - output);
                d_hidden *= sum;
                neuron *from = c->from;
                float d_weight = from->out * d_hidden;
                conn_adjust_weight(c, LEARNING * d_weight);
            }
        }
    }

    return result;
}
