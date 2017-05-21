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


void test_feedforward(test_t *t) {
    struct {
        int    num_layers;
        int    *sizes;
        double *weights;
        double *biases;
        double *in, *expected_out;
        double err;
    }
    tests[] = {
        {
            .num_layers = 3,
            .sizes      = (int[]){2, 3, 2 },
            .weights    = (double[]){
                1, 2, -3, 6, 7, -4,  // weights[0], 2*3
                -6, 3, -1, 4, 2, -12 // weights[1], 3*2
            },
            .biases = (double[]){
                6, 4, 2, // biases[0]
                2, 5     // biases[1]
            },
            .in           = (double[]){1, 0 },
            .expected_out = (double[]){                 0.0572575,0.17652854 },
            .err          = 1e-6
        }
    };

    for (int i = 0; i < ARRAY_LEN(tests); i++) {
        neuralnet_t *net = make_neuralnet(tests[i].num_layers, tests[i].sizes);

        int weights_offset = 0;
        int biases_offset  = 0;
        for (int j = 0; j < tests[i].num_layers - 1; j++) {
            matrix_initialize_from_values(net->weights[j],
                                          net->weights[j]->num_rows,
                                          net->weights[j]->num_cols,
                                          tests[i].weights + weights_offset);
            weights_offset += net->weights[j]->num_rows *
                              net->weights[j]->num_cols;


            matrix_initialize_from_values(net->biases[j],
                                          net->biases[j]->num_rows,
                                          net->biases[j]->num_cols,
                                          tests[i].biases + biases_offset);
            biases_offset += net->biases[j]->num_rows *
                             net->biases[j]->num_cols;
        }

        matrix_t *in = make_matrix(0, 0, 0);
        matrix_initialize_from_values(in,
                                      tests[i].sizes[0], 1,
                                      tests[i].in);

        matrix_t *out_got      = make_matrix(0, 0, 0);
        matrix_t *out_expected = make_matrix(0, 0, 0);
        matrix_initialize_from_values(out_expected,
                                      tests[i].sizes[tests[i].num_layers - 1], 1,
                                      tests[i].expected_out);

        neuralnet_feedforward(net, in, out_got);
        CHECK_MATRIX_EQUAL(out_expected, out_got, tests[i].err,
                           __FILE__, __LINE__);

        free_neuralnet(net);
    }
}


int main(int argc, char **argv) {
    test_function_t tests[] = {
        TEST_FUNCTION(test_neuralnet_creation),
        TEST_FUNCTION(test_save_and_load),
        TEST_FUNCTION(test_feedforward)
    };

    srand(time(NULL));

    test_run(tests, ARRAY_LEN(tests));
}
