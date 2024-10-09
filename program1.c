#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <gsl/gsl_permutation.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
#include <gsl/gsl_statistics.h>
#include <time.h>

// typedef struct necessary_packet
// {
//     size_t piv;
//     unsigned int comp_no;
// } packet;

// packet pkt;

unsigned long long int number_of_comparisions = 0;

double mean(size_t * a, size_t N) {

    double mean = 0;

    for (size_t i = 0; i < N; i++)
    {
        mean += a[i];
    }
    
    return mean/(1.0 * N);
}


int Is_Sorted(int* A, int N){
    int tempcr, tempdcr;

    for (int i=0; i<N-1; i++){

        if (A[i]<=A[i+1]){
            tempcr++;
        }else if (A[i]>=A[i+1]){
        tempdcr++;
        }
    }
    if(tempcr==N-1){
        return 1;
    }else if(tempdcr==N-1)
        return -1;
    else
        return 0;

}

size_t partition(size_t * a, long long int p, long long int r) {

    // pkt.piv = -1;
    // pkt.comp_no = 0;

    // Taking current time as seed
    unsigned int seed = time(0);

    // randomized pivot
    long long int pivot_index = (rand_r(&seed) % (r-p+1)) + p;
    // swap the pivot element with the last element 
    size_t temp = a[pivot_index];
    a[pivot_index] = a[r];
    a[r] = temp;

    long long int pivot = a[r];
    long long int i = p-1, j;

    for (long long int j = p; j < r; j++)
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


void quicksort(size_t * a, long long int p, long long int r) {

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
    int sizes[] = {pow(10,1), pow(10,2), pow(10,3), pow(10,4)};
    const gsl_rng_type * T;
    gsl_rng * r;
    size_t N;

    gsl_rng_env_setup();
    T = gsl_rng_default;
    r = gsl_rng_alloc(T);

    unsigned int seed = time(0);

    int any_number = rand_r(&seed) % 4;
    printf("%d %d \n", any_number, sizes[any_number]);

    N = sizes[any_number];

    gsl_permutation * p = gsl_permutation_alloc(N);
    gsl_permutation_init(p);

    double * collection = (double *)malloc(sizeof(double) * 100);

    FILE * record = fopen("record.txt", "a+");
    if (record == NULL) {
        printf("Error opening file.\n");
        return 1;
    }

    for (i=0; i<100; i++) {

        // int any_number = rand() % 4;
        // printf("%d %d \n", any_number, sizes[any_number]);

        // N = sizes[any_number];
        // N = 100;

        // moved out permutation creation and initialization

        // moved the random number generator creation from inside of for loop to outside

        gsl_ran_shuffle (r, p->data, N, sizeof(size_t));

        gsl_permutation_fprintf(record, p, " %u");

        quicksort(p->data, 0, N-1);
        collection[i] = number_of_comparisions;
        printf("\nno of comp for %dth iteration is: %llu\n", i, number_of_comparisions);

        fprintf(record, "\nCorresponding sorted array:\n");
        gsl_permutation_fprintf(record, p, " %u");

        number_of_comparisions = 0;

        // moved freeing up permutation resources

        fprintf(record, "\n");
    }

    // mean and SD calculation:
    double mean_of_all_no_of_comparisions = gsl_stats_mean(collection, 1, 100);
    double standard_deviation_of_all_no_of_comparisions = gsl_stats_sd(collection, 1, 100);

    printf("the mean and SD of the no of comparisions is: %g %g\n", mean_of_all_no_of_comparisions, standard_deviation_of_all_no_of_comparisions);

    free(collection);

    gsl_permutation_free(p);
    gsl_rng_free(r);
}