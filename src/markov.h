/***
 * markov.h
 *
 * Project-wide convenience functions, macros, and header files.
 *
 */

#ifndef __MARKOV_H
#define __MARKOV_H

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h> /* time() will be the random seed */

typedef int boolean;
#define TRUE 1
#define FALSE 0

typedef void * data;

#include "graph.h"
#include "coro.h"
#include "model.h"

void markov_init();
double markov_rand();

#endif /* __MARKOV_H */
