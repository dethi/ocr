#include <stdio.h>
#include <stdlib.h>
#include "nn.h"

int main()
{
    size_t n = 3;
    size_t desc_layers[] = { 2, 2, 1 };
    double inputs[] = { 1, 0 };

    struct net nwk = net_init(n, desc_layers);
    net_compute(nwk, inputs);
    printf("Result: %f\n", nwk.layers[nwk.n_layer-1].out[0]);

    net_train(nwk);
    net_free(nwk);

    return EXIT_SUCCESS;
}
