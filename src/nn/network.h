#ifndef _NETWORK_H_
#define _NETWORK_H_

typedef struct net {
    struct neuron input[];
    struct neuron hidden[];
    struct neuron output;

    void (*free) (struct net*);
} net;

#endif
