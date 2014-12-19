#ifndef __MODEL_H
#define __MODEL_H

#include "markov.h"

typedef struct {
    double score;
    uint32_t dest;
} Neighbor;

typedef struct {
    Neighbor *edges;
    size_t size;
} State;

typedef struct {
    State **states;
    size_t size;
} Model;

Model * new_model_from_graph(Graph *gr);
void del_model(Model *m);
uint32_t random_neighbor(Model *m, uint32_t src);
uint32_t walk(Model *m);
Model *serialize_model(Model *m, redisContext *ctx, char *key);

#endif /* __MODEL_H */

