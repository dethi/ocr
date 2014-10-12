#ifndef _NEURON_H_
#define _NEURON_H_

typedef struct neuron {
    float output;
    connec *links;
    size_t nlinks;
    int bias; //BOOL

    void (*calc_out) (struct neuron*);
    void (*add_link) (struct neuron*, connec*);
    void (*free) (struct neuron*);
} neuron;

neuron* new_neuron(int bias);
void neuron_calc_out(neuron *this);
void neuron_add_link(neuron *this, connec *l);
void neuron_free(neuron *this);

#endif
