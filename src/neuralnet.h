#ifndef __NEURALNET_H__
#define __NEURALNET_H__

#include "matrix.h"

// neuralnet_t represents a neural network.
typedef struct {
    int      num_layers;
    int      *sizes;
    matrix_t *weights;
    matrix_t *biases;
} neuralnet_t;

// make_neuralnet alloctate and initialize a new neural network with the given
// number of layers and sizes. Sizes is a table of length num_layers containing
// the size of each layer.
neuralnet_t *make_neuralnet(int num_layers, int *sizes);
void free_neuralnet(neuralnet_t *net);

// load_neuralnet loads a neural network from the given file.
// It takes:
//    - a pointer to a pointer to a neural network to which will point to
//      the resulting neural network.
//    - a file from which to load the neural network.
// It returns 0 on success.
int load_neuralnet(neuralnet_t **net, char *filename);

// neuralnet_save saves the given neural network to the given file.
// It returns 0 on success.
int neuralnet_save(neuralnet_t *net, char *filename);

// neuralnet_randomize randomizes the weights and biases with a normal
// distribution.
void neuralnet_randomize(neuralnet_t *net);

// neuralnet_feedforward computes the output from the given input through the
// neural network.
int neuralnet_feedforward(neuralnet_t *net, matrix_t *in, matrix_t *out);

#endif
