#include <stdlib.h>
#include <string.h>

#include "neuralnet.h"
#include "matrix.h"
#include "randn.h"

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
    exit(-1); // TODO: Implement
    return 0;
}


int neuralnet_save(neuralnet_t *net, char *filename) {
    exit(-1); // TODO: Implement
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


int neuralnet_feedforward(neuralnet_t *net,
                          matrix_t *in, matrix_t *out) {
    exit(-1); // TODO: Implement
    return 0;
}
