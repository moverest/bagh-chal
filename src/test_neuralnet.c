#include <stdio.h>
#include <stdlib.h>

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


int main(int argc, char **argv) {
    test_function_t tests[] = {
        TEST_FUNCTION(test_neuralnet_creation)
    };

    test_run(tests, ARRAY_LEN(tests));
}
