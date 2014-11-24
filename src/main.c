#include "markov.h"

int main(int argc, char **argv) {

    /* Initialize the application */
    markov_init();

    /* Construct a graph */
    Graph *gr = new_graph(3);
    gr = add_edge(gr, 0, 2, 1.0);
    gr = add_edge(gr, 1, 2, 0.5);
    gr = add_edge(gr, 1, 0, 0.2);
    gr = add_edge(gr, 2, 1, 0.4);

    gr = add_vertex(gr,NULL);
    gr = add_edge(gr, 2, 3, 0.6);
    gr = add_edge(gr, 1, 3, 0.3);
    gr = add_edge(gr, 3, 0, 1.0);

    gr = print_graph(gr);
    Model *m = new_model_from_graph(gr);

    /* Random walk through the graph based on edge weights */
    printf("100-step walk starting with 0\n*****\n");
    uint32_t idx = -1;
    int i;
    for (i = 0; i < 100; i++) {
        idx = walk(m);
        printf(" -> %d", idx);
    }
    printf("\n");

    del_model(m);
    del_graph(gr);
    return 0;
}

