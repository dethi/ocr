#include <stdio.h>
#include <stdlib.h>
#include "nn.h"

int main()
{
    size_t n = 2;
    size_t desc_layers[] = { 2, 1 };

    struct net nwk = net_init(n, desc_layers);
    net_train(nwk);
    net_save(nwk, "xor.saved");
    net_free(nwk);

    return EXIT_SUCCESS;
}
