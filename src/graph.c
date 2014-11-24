/***
 * graph.c
 *
 * Implementation of an adjacency list-based graph
 *
 * TODO
 * 1. Serializers and deserializers for graphs
 * 2. Extend EdgeNode to contain probability data
 * 3. Function to select a random neighbor based on probability
 */
#include "markov.h"

EdgeNode *new_edge_node(uint32_t lbl) {
    EdgeNode *n = (EdgeNode *) malloc(sizeof(EdgeNode));
    n->label = lbl;
    n->next  = NULL;
    n->prob  = 0.0;
    return n;
}

/* Frees the node and returns its successor */
EdgeNode *del_edge_node(EdgeNode *n) {
    EdgeNode *successor = n->next;
    free(n);
    return successor;
}

/* Iterates through the list and frees list nodes */
void del_list(EdgeNode *head) {
    if (head != NULL) { /* Don't free NULL pointers */
        EdgeNode *cur = head;
        while (cur) {
            cur = del_edge_node(cur);
        }
    }
    return;
}

/* Prepend a EdgeNode to another (think cons in lisp) */
EdgeNode *prepend(EdgeNode *n, EdgeNode *lst) {
    if (lst != NULL) { /* Don't access NULL pointers */
        n->next = lst;
    }
    return n;
}

EdgeNode *print_list(EdgeNode *hd) {
    EdgeNode *cur = hd;
    while (cur) {
        printf("%d with prob %f\n", cur->label,cur->prob);
        cur = cur->next;
    }
    return hd;
}

Vertex *new_vertex(data d) {
    Vertex *v = (Vertex *) malloc(sizeof(Vertex));
    v->num_edges = 0;
    v->edges = NULL;
    v->payload = d;
    v->label = 0;
    return v;
}

void del_vertex(Vertex *v) {
    del_list(v->edges ); /* Free the adjacency list */
    if (v->payload != NULL) { /* Don't free NULL pointers */
        free(v->payload);
    }
    free (v);
    return;
}

Vertex **vertex_edges(Graph *gr, uint32_t vertex_idx) {
    Vertex *v = gr->vertices[vertex_idx];
    Vertex **vs = NULL;
    if (v->num_edges) {
        Vertex **vs = (Vertex **)malloc(v->num_edges * sizeof(Vertex *));
        EdgeNode *cur = v->edges;
        int i = 0;
        while (cur) {
            vs[i] = gr->vertices[cur->label];
            cur = cur->next;
            i = i + 1;
        }
    }
    return vs;
}

/* A Graph is an array of EdgeNodes */
Graph *new_graph(uint32_t sz) {
    int i;

    Graph *gr = (Graph *) malloc(sizeof(Graph));
    gr->size = sz;
    gr->vertices = (Vertex **) malloc(sz * sizeof(Vertex *));

    for (i = 0; i < sz; i++) {
        gr->vertices[i] = new_vertex(NULL);
        gr->vertices[i]->label = i;
    }

    return gr;
}

void del_graph(Graph *gr) {
    /* Iterate through the vertices and free them */
    int i;
    for (i = 0; i < gr->size; i++) {
        del_vertex(gr->vertices[i]);
    }
    /* Destroy the array of vertices itself along with the Graph */
    free(gr->vertices);
    free(gr);
    return;
}

Graph *add_vertex(Graph *gr, data pyld) {
    /* Reallocate the vertices array to contain one more Vertex */
    gr->vertices = (Vertex **) realloc(gr->vertices,
            (gr->size + 1) * sizeof(Vertex *));

    /* Create the new Vertex and bump the Graph's size */
    gr->vertices[gr->size] = new_vertex(pyld);
    gr->vertices[gr->size]->label = gr->size;
    gr->size = gr->size + 1;
    return gr;
}

Graph *add_edge(Graph *gr, uint32_t src, uint32_t dst, double prb) {
    EdgeNode *dst_node = new_edge_node(dst);
    dst_node->prob = prb;
    Vertex *src_vertex = gr->vertices[src];
    if (src_vertex != NULL) { /* Don't access NULL pointers */
        src_vertex->edges= prepend(dst_node, src_vertex->edges);
        src_vertex->num_edges = src_vertex->num_edges + 1;
    }
    return gr;
}

Graph *print_graph(Graph *gr) {
    int i;
    for (i = 0; i < gr->size; i++) {
        Vertex *v = gr->vertices[i];
        printf("Outbound from vertex %d:\n", v->label);
        print_list(v->edges);
        printf("\n");
    }
    return gr;
}
