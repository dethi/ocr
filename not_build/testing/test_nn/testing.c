#include <stdio.h>
#include <stdlib.h>
#include "nn.h"

int main()
{
    size_t n = 3;
    size_t desc_layers[] = { 2, 2, 1 };

    struct training t = { NULL, NULL, 4, 2, 1 };
    double in[] = {
        0, 0,
        0, 1,
        1, 0,
        1, 1,
    };
    double out[] = { 0, 1, 1, 0 };
    t.in = in;
    t.out = out;

    /* Test training */
    struct net nwk = net_init(n, desc_layers);
    net_train(nwk, t);
    net_save(nwk, "xor.saved");
    net_free(nwk);

    /* Test loading file */
    nwk = net_load("xor.saved");
    double inputs[] = { 0, 1 };
    net_compute(nwk, inputs);
    printf("\n\nRESULT: %.2f xor %.2f = %.2f\n", inputs[0], inputs[1],
            nwk.layers[nwk.n_layer-1].out[0]);
    net_free(nwk);

    return EXIT_SUCCESS;
}
