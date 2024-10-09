#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <gsl/gsl_permutation.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>

// typedef struct necessary_packet
// {
//     size_t piv;
//     unsigned int comp_no;
// } packet;

// packet pkt;

unsigned long long int number_of_comparisions = 0;

size_t partition(size_t * a, size_t p, size_t r) {

    // pkt.piv = -1;
    // pkt.comp_no = 0;

    size_t pivot = a[r];
    size_t i = p-1, j;

    for (size_t j = p; j < r; j++)
    {
        // pkt.comp_no = pkt.comp_no + 1;
        number_of_comparisions++;
        if (a[j] <= pivot) {
            i++;
            // exchange a[i] and a[j]
            size_t temp = a[i];
            a[i] = a[j];
            a[j] = temp;
        }
    }
    // swap a[i+1] and a[r]
    a[r] = a[i+1];
    a[i+1] = pivot;
    // pkt.piv = a[i+1];
    return i+1;
}


void quicksort(size_t * a, size_t p, size_t r) {

    // unsigned int number_of_comparisions = 0;

    if (p < r) {
        // packet one_pkt = partition(a, p, r);
        size_t q = partition(a, p, r);
        // number_of_comparisions += one_pkt.comp_no;
        quicksort(a, p, q-1);
        quicksort(a, q+1, r);
    }
}

int main() {

    int i;
    int sizes[] = {pow(10,4), pow(10,5), pow(10,6), pow(10,7)};
    const gsl_rng_type * T;
    gsl_rng * r;
    size_t N;

    unsigned int * collection = (unsigned int *)malloc(sizeof(unsigned int) * 100);

    FILE *record = fopen("record.txt", "a+");
    if (record == NULL) {
        printf("Error opening file.\n");
        return 1;
    }

    for (i=0; i<100; i++) {

        int any_number = rand() % 4;
        printf("%d %d \n", any_number, sizes[any_number]);

        N = sizes[any_number];

        gsl_permutation * p = gsl_permutation_alloc(N);
        gsl_permutation_init(p);

        gsl_rng_env_setup();
        T = gsl_rng_default;
        r = gsl_rng_alloc(T);

        gsl_ran_shuffle (r, p->data, N, sizeof(size_t));

        gsl_permutation_fprintf(record, p, " %u");

        quicksort(p->data, 0, N-1);
        collection[i] = number_of_comparisions;
        printf("no of comp for %dth iteration is: %llu", i, number_of_comparisions);
        number_of_comparisions = 0;

        gsl_permutation_free(p);
        gsl_rng_free(r);
        fprintf(record, "\n");
    }
    free(collection);
}