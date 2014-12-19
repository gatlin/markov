#include "markov.h"

Model * new_model_from_graph(Graph *gr) {
    Model *m = (Model *) malloc(sizeof(Model));
    m->states = (State **) malloc(gr->size * sizeof(State *));
    m->size = gr->size;
    int i, j;
    double check;
    for (i = 0; i < gr->size; i++) {
        m->states[i] = (State *) malloc(sizeof(State));
        m->states[i]->size = gr->vertices[i]->num_edges;
        m->states[i]->edges = (Neighbor *) malloc(m->states[i]->size *
                sizeof(Neighbor));
        check = 0.0;
        j = 0;
        EdgeNode *e = gr->vertices[i]->edges;
        while (e) {
            Neighbor n;
            check = check + e->prob;
            n.score = check;
            n.dest  = e->label;
            m->states[i]->edges[j] = n;
            e = e->next;
            j = j + 1;
        }
    }
    return m;
}

void del_model(Model *m) {
    int i,j;
    for (i = 0; i < m->size; i++) {
        free(m->states[i]->edges);
        free(m->states[i]);
    }
    free(m->states);
    free(m);
    return;
}

uint32_t random_neighbor(Model *m, uint32_t src) {
    double r = markov_rand();
    double lower_bound = 0.0;
    int i;
    Neighbor n;
    for (i = 0; i < m->states[src]->size; i++) {
        n = m->states[src]->edges[i];
        if (r > lower_bound && r <= n.score) {
            return n.dest;
        }
        else {
            lower_bound = n.score;
        }
    }
    return -1;
}

uint32_t walk(Model *m) {
    static uint32_t idx = 0;
    coro_start;
    while(TRUE) {
        idx = random_neighbor(m, idx);
        yield(idx);
    }
    coro_finish;
}

Model *serialize_model(Model *m, redisContext *ctx, char *key) {
    redisReply *reply;
    uint64_t edge, v1, v2;
    int i, j;
    reply = redisCommand(ctx, "HSET %s size %d",
            key,
            m->size);
    freeReplyObject(reply);
    for (i = 0; i < m->size; i++) {
        v1 = (uint64_t) i;
        for (j = 0; j < m->states[i]->size; j++) {
            v2 = (uint64_t) m->states[i]->edges[j].dest;
            printf("Wrote edge %lu -> %lu\n", v1, v2);
            edge = ((v1 << 32 | v2));
            reply = redisCommand(ctx, "HSET %s %lu %f",
                    key,
                    edge,
                    m->states[i]->edges[j].score);
            freeReplyObject(reply);
        }
    }
    return m;
}
