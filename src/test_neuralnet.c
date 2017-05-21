#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "test.h"
#include "tools.h"
#include "matrix.h"
#include "neuralnet.h"
#include "test_matrix.h"

void test_neuralnet_creation(test_t *t) {
    int         sizes[] = { 4, 6, 2 };
    neuralnet_t *net    = make_neuralnet(ARRAY_LEN(sizes), sizes);

    for (int i = 0; i < net->num_layers - 1; i++) {
        CHECK_MATRIX_SIZE(net->weights[i], sizes[i + 1], sizes[i], -1,
                          __FILE__, __LINE__);
    }

    neuralnet_randomize(net);
    for (int i = 0; i < net->num_layers - 1; i++) {
        printf("weights[%d]: ", i);
        print_matrix(net->weights[i]);
        printf("biases[%d]: ", i);
        print_matrix(net->biases[i]);
    }

    free(net);
}


#define TEST_SAVE_AND_LOAD_FILENAME    "/tmp/test_neuralnet_save_and_load"

void test_save_and_load(test_t *t) {
    int         sizes[] = { 4, 6, 2 };
    neuralnet_t *net1   = make_neuralnet(ARRAY_LEN(sizes), sizes);
    neuralnet_t *net2;
    int         err;

    neuralnet_randomize(net1);
    err = neuralnet_save(net1, TEST_SAVE_AND_LOAD_FILENAME);
    if (err != 0) {
        printf("Error %d while saving.\n", err);
        test_fail(t);
    }

    err = load_neuralnet(&net2, TEST_SAVE_AND_LOAD_FILENAME);
    if (err != 0) {
        printf("Error %d while loading.\n", err);
        test_fail(t);
    }

    if (net1->num_layers != net2->num_layers) {
        printf("num_layers does not match.\n");
        test_fail(t);
    }

    for (int i = 0; i < net1->num_layers; i++) {
        if (net1->sizes[i] != net2->sizes[i]) {
            printf("sizes[%d] does not match.\n", i);
            test_fail(t);
        }
    }

    for (int i = 0; i < net1->num_layers - 1; i++) {
        if (!matrix_equals(net1->weights[i], net2->weights[i], .0000001)) {
            printf("weights[%d] does not match\n", i);
            test_fail(t);
        }


        if (!matrix_equals(net1->biases[i], net2->biases[i], .0000001)) {
            printf("biases[%d] does not match\n", i);
            test_fail(t);
        }
    }

    free(net1);
    free(net2);
}


int main(int argc, char **argv) {
    test_function_t tests[] = {
        TEST_FUNCTION(test_neuralnet_creation),
        TEST_FUNCTION(test_save_and_load)
    };

    srand(time(NULL));

    test_run(tests, ARRAY_LEN(tests));
}
