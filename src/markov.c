/***
 * markov.c
 *
 * Contains application initialization functions
 */

#include "markov.h"

/* For now, this function simply sets the random seed */
void markov_init() {
    srand(time(NULL));
}

double markov_rand() {
    return (double) rand() / (double) RAND_MAX;
}
