#include <stdbool.h>
#include <stdlib.h>
#include <math.h>

#include "randn.h"

// See: https://en.wikipedia.org/wiki/Marsaglia_polar_method
double randn() {
    static bool   reserve_ready = false;
    static double reserve;

    if (reserve_ready) {
        reserve_ready = false;
        return reserve;
    }

    double u1, u2, w;
    do {
        u1 = ((double)rand() / RAND_MAX) * 2. - 1.;
        u2 = ((double)rand() / RAND_MAX) * 2. - 1.;
        w  = u1 * u1 + u2 * u2;
    } while (w >= 1 || w == 0);

    reserve       = u1 * sqrt((-2 * log(w)) / w);
    reserve_ready = true;

    return u2 * sqrt(-2 * log(w) / w);
}
