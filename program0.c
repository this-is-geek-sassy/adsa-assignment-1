#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <gsl/gsl_permutation.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
#include <gsl/gsl_statistics.h>
#include <time.h>

int main() {
    int i;
    int sizes[] = {pow(10,4), pow(10,5), pow(10,6), pow(10,7)};
    const gsl_rng_type * T;
    gsl_rng * r;
    size_t N;

    gsl_rng_env_setup();
    T = gsl_rng_default;
    r = gsl_rng_alloc(T);

    unsigned int seed = time(0);

    int any_number = rand_r(&seed) % 4;
    printf("%d %d \n", any_number, sizes[any_number]);

    N = sizes[3];

    gsl_permutation * p = gsl_permutation_alloc(N);
    gsl_permutation_init(p);

    double * collection = (double *)malloc(sizeof(double) * 100);

    

    for (i=0; i<100; i++) {

        char filename[256];
        snprintf(filename, sizeof(filename), "random_array_%lu_%d.txt", N, i + 1);

        FILE * record = fopen(filename, "a+");
        if (record == NULL) {
            printf("Error opening file.\n");
            return 1;
        }

        // int any_number = rand() % 4;
        // printf("%d %d \n", any_number, sizes[any_number]);

        // N = sizes[any_number];
        // N = 100;

        gsl_ran_shuffle (r, p->data, N, sizeof(size_t));

        gsl_permutation_fprintf(record, p, " %u");


        fprintf(record, "\n");
    }

    free(collection);

    gsl_permutation_free(p);
    gsl_rng_free(r);

}