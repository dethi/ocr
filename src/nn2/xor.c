#include "xor.h"

static inline
double f(double x)
{
    return 1.0 / (1.0 + exp(-x));
}

static inline
double df(double x)
{
    return x * (1 - x);
}

int main()
{
    struct net net;
    train(&net);

    int in1 = -1;
    int in2 = -1;

    while (1) {
        printf("\n");

        printf("Input 1 [0/1] : ");
        scanf("%d", &in1);
        if (in1 != 0 && in1 != 1)
            return EXIT_SUCCESS;

        printf("Input 2 [0/1] : ");
        scanf("%d", &in2);
        if (in2 != 0 && in2 != 1)
            return EXIT_SUCCESS;

        printf("Result: %d\n",
                (net_calc_output(&net, in1, in2) > 0.5) ? 1 : 0);
    }
}

void train(struct net *net)
{
    double inputs[] =
    {
        0, 0,
        0, 1,
        1, 0,
        1, 1
    };

    double results[4] = { 0, 1, 1, 0 };

    net_randomize_w(net);

    int epoch = 0;
    double error;

    do {
        ++epoch;
        error = 0;
        printf("[epoch %d]\n", epoch);

        for (int i = 0; i < 4; ++i) {
            double out = net_calc_output(net, inputs[2*i], inputs[2*i+1]);

            printf("\t%f xor %f = %d (%f)\n", inputs[2 * i], inputs[2 * i + 1],
                    (out > 0.5) ? 1 : 0, out);

            // Back propagation
            net->output.error = df(out) * (results[i] - out);
            neuron_adjust_w(&net->output);

            net->hidden1.error = df(neuron_calc_output(&net->hidden1)) *
                net->output.error * net->output.w[0];
            net->hidden2.error = df(neuron_calc_output(&net->hidden2)) *
                net->output.error * net->output.w[1];

            neuron_adjust_w(&net->hidden1);
            neuron_adjust_w(&net->hidden2);

            error += abs(((out > 0.5) ? 1 : 0) - results[i]);
        }

        if (error && (epoch % 2000) == 0)
            net_randomize_w(net);

    } while (error);
}

void net_randomize_w(struct net *this)
{
    neuron_randomize_w(&this->hidden1);
    neuron_randomize_w(&this->hidden2);
    neuron_randomize_w(&this->output);
}

double net_calc_output(struct net *this, double in1, double in2)
{
    this->hidden1.in[0] = in1;
    this->hidden1.in[1] = in2;
    this->hidden2.in[0] = in1;
    this->hidden2.in[1] = in2;

    this->output.in[0] = neuron_calc_output(&this->hidden1);
    this->output.in[1] = neuron_calc_output(&this->hidden2);
    return neuron_calc_output(&this->output);
}

double neuron_calc_output(struct neuron *this)
{
    return f(this->w[0] * this->in[0] +
            this->w[1] * this->in[1] + this->bias);
}

void neuron_randomize_w(struct neuron *this)
{
    srand(time(NULL));

    this->w[0] = (double)rand() / (double)RAND_MAX;
    this->w[1] = (double)rand() / (double)RAND_MAX;
    this->bias = (double)rand() / (double)RAND_MAX;
}

void neuron_adjust_w(struct neuron *this)
{
    this->w[0] += this->error * this->in[0];
    this->w[1] += this->error * this->in[1];
    this->bias += this->error;
}
