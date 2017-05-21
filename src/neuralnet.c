#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <math.h>

#include "neuralnet.h"
#include "matrix.h"
#include "randn.h"

// FILE_FORMAT_MAGIC_KEY is used to check the file format in which neural
// networks are saved before loading it.
#define FILE_FORMAT_MAGIC_KEY    0x434d4e54

// The networks are stored in a binary file.
// Format:
//   uint32: magic key
//   uint32: num_layers
//   uint32: size[0]
//   ...
//   uint32: size[num_layers - 1]
//   double: values weights[0]
//   ...
//   double: values weights[num_layers - 2]
//   double: values biases[0]
//   ...
//   double: values biases[num_layers - 2]

neuralnet_t *make_neuralnet(int num_layers, int *sizes) {
    neuralnet_t *net = malloc(sizeof(neuralnet_t));

    if (net == NULL) {
        return NULL;
    }

    net->num_layers = num_layers;
    net->sizes      = malloc(sizeof(int) * num_layers);
    if (net->sizes == NULL) {
        free(net);
        return NULL;
    }
    memcpy(net->sizes, sizes, num_layers * sizeof(int));

    net->weights = malloc(sizeof(matrix_t *) * (num_layers - 1));
    if (net->weights == NULL) {
        free_neuralnet(net);
        return NULL;
    }

    net->biases = malloc(sizeof(matrix_t *) * (num_layers - 1));
    if (net->biases == NULL) {
        free_neuralnet(net);
        return NULL;
    }

    for (int i = 0; i < num_layers - 1; i++) {
        net->weights[i] = make_matrix(sizes[i + 1], sizes[i], 0);
        net->biases[i]  = make_matrix(sizes[i + 1], 1, 0);

        if ((net->weights[i] == NULL) || (net->biases[i] == NULL)) {
            free_neuralnet(net);
            return NULL;
        }
    }

    return net;
}


void free_neuralnet(neuralnet_t *net) {
    if (net->biases != NULL) {
        for (int i = 0;
             i < net->num_layers - 1 && net->biases[i] != NULL;
             i++) {
            free_matrix(net->biases[i]);
        }
        free(net->biases);
    }

    if (net->weights != NULL) {
        for (int i = 0;
             i < net->num_layers - 1 && net->weights[i] != NULL;
             i++) {
            free_matrix(net->weights[i]);
        }
        free(net->weights);
    }

    if (net->biases != NULL) {
        free(net->sizes);
    }

    free(net);
}


int load_neuralnet(neuralnet_t **net, char *filename) {
    // FIXME: Platform dependent. On x86 Platforms, this saves the file in
    // little-endien. This should not be platform dependent.

    // FIXME: Needs to handle errors better.

    neuralnet_t *network = malloc(sizeof(neuralnet_t));

    if (network == NULL) {
        return 1;
    }

    FILE *f = fopen(filename, "rb");

    if (f == NULL) {
        return -1;
    }

    uint32_t value;
    fread(&value, sizeof(value), 1, f);
    if (value != FILE_FORMAT_MAGIC_KEY) {
        return -2;
    }

    fread(&value, sizeof(value), 1, f);
    network->num_layers = value;

    network->sizes = malloc(sizeof(int) * network->num_layers);
    if (network->sizes == NULL) {
        free_neuralnet(network);
        return 2;
    }

    for (int i = 0; i < network->num_layers; i++) {
        fread(&value, sizeof(value), 1, f);
        network->sizes[i] = value;
    }

    network->weights = malloc(sizeof(matrix_t) * (network->num_layers - 1));
    if (network->weights == NULL) {
        free_neuralnet(network);
        return 3;
    }

    for (int i = 0; i < network->num_layers - 1; i++) {
        network->weights[i] = make_matrix(network->sizes[i + 1], network->sizes[i], 0);
        size_t size = network->weights[i]->num_cols * network->weights[i]->num_rows;
        fread(&(network->weights[i]->values), size * sizeof(double), 1, f);
    }

    network->biases = malloc(sizeof(matrix_t) * (network->num_layers - 1));
    if (network->biases == NULL) {
        free_neuralnet(network);
        return 4;
    }

    for (int i = 0; i < network->num_layers - 1; i++) {
        network->biases[i] = make_matrix(network->sizes[i + 1], 1, 0);
        size_t size = network->biases[i]->num_cols * network->biases[i]->num_rows;
        fread(&(network->biases[i]->values), size * sizeof(double), 1, f);
    }

    fclose(f);
    *net = network;
    return 0;
}


int neuralnet_save(neuralnet_t *net, char *filename) {
    // FIXME: Platform dependent. On x86 Platforms, this saves the file in
    // little-endien. This should not be platform dependent.

    // FIXME: Needs to handle errors better.

    FILE *f = fopen(filename, "wb");

    if (f == NULL) {
        return -1;
    }

    uint32_t value = FILE_FORMAT_MAGIC_KEY;
    fwrite(&value, sizeof(value), 1, f);

    value = net->num_layers;
    fwrite(&value, sizeof(value), 1, f);

    for (int i = 0; i < net->num_layers; i++) {
        value = net->sizes[i];
        fwrite(&value, sizeof(value), 1, f);
    }

    for (int i = 0; i < net->num_layers - 1; i++) {
        size_t size = net->weights[i]->num_cols * net->weights[i]->num_rows;
        fwrite(&net->weights[i]->values, size * sizeof(double), 1, f);
    }

    for (int i = 0; i < net->num_layers - 1; i++) {
        size_t size = net->biases[i]->num_cols * net->biases[i]->num_rows;
        fwrite(&net->biases[i]->values, size * sizeof(double), 1, f);
    }

    fclose(f);
    return 0;
}


static double randn_f(double v) {
    return randn();
}


void neuralnet_randomize(neuralnet_t *net) {
    for (int i = 0; i < net->num_layers - 1; i++) {
        matrix_apply(net->weights[i], net->weights[i], randn_f);
        matrix_apply(net->biases[i], net->biases[i], randn_f);
    }
}


static double sigmoid(double x) {
    return 1. / (1. + exp(-x));
}


int neuralnet_feedforward(neuralnet_t *net,
                          matrix_t *in, matrix_t *out) {
    matrix_t *temp = make_matrix(0, 0, 0);

    matrix_copy(in, out);
    for (int i = 0; i < net->num_layers - 1; i++) {
        matrix_product(net->weights[i], out, temp);
        matrix_add(temp, net->biases[i], out);
        matrix_apply(out, out, sigmoid);
    }

    free_matrix(temp);

    return 0;
}
