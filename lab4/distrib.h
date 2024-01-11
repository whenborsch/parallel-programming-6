#include "xst64.h"
#include <cmath>
#include <cstdlib>

double xst64_normal(xst64 &generator, double mu, double sigma) {
    double u1 = generator() * (1.0 / RAND_MAX);
    double u2 = generator() * (1.0 / RAND_MAX);
    double z = sqrt(-2.0 * log(u1)) * cos(2.0 * M_PI * u2);
    return mu + sigma * z;
}

double xst64_binomial(xst64 &generator) {
    int k = 0;

    for(int i = 0; i < 1000; i++) {
        if((generator() / (double)RAND_MAX) < 0.5) {
            k++;
        }
    }

    return k;
}

double xst64_exp(xst64 &generator, double lambda) {
    double u = generator() * (1.0 / RAND_MAX);
    return -log(1 - u) / lambda;
}

int xst64_poisson(xst64 &generator, double lambda) {
    int k = 0;
    double sum = 0.0;

    while (sum < lambda) {
        sum += -log(1 - generator() * (1.0 / RAND_MAX));
        k++;
    }

    return k - 1;
}

double xst64_weibull(xst64 &generator, double lambda, double k) {
    double u = generator() * (1.0 / RAND_MAX);
    return lambda * pow(-log(1 - u), 1.0 / k);
}