/***
 * graph.h
 *
 * Adjacency list-based graph data structure and some operations
 *
 * Many functions both accept and return a Graph pointer. This encourages a
 * style of programming wherein if you give a pointer to a function you must
 * receive it back in order to keep using it. This can aid in reasoning about
 * code and potentially allow a tool to do linearity checks.
 */

#ifndef __GRAPH_H
#define __GRAPH_H

#include "markov.h"

typedef struct _EdgeNode EdgeNode;

/* A EdgeNode contains a label and a pointer to another EdgeNode. */
struct _EdgeNode {
    uint32_t label;   /* Numeric label */
    double   prob;    /* Probability of this edge */
    EdgeNode *next;   /* Pointer to the next list node */
};

EdgeNode *new_edge_node(uint32_t lbl);
EdgeNode *del_edge_node(EdgeNode *n);
void del_edges(EdgeNode *head);
EdgeNode *prepend(EdgeNode *n, EdgeNode *lst);
EdgeNode *print_list(EdgeNode *hd);

/* A Vertex is an EdgeNode pointer and an arbitrary payload. The linked list
 * contains all outbound edges. */
typedef struct {
    uint32_t  num_edges;
    EdgeNode *edges;
    uint32_t  label;
    data      payload;
} Vertex;

Vertex *new_vertex(data d);
void del_vertex(Vertex *v);

/* A Graph is an array of Vertices. */
typedef struct {
    uint32_t size;     /* number of vertices */
    Vertex **vertices; /* adjacency list of edges for each vertex. */
} Graph;

Graph *new_graph(uint32_t sz);
void del_graph(Graph *gr);
Graph *add_vertex(Graph *gr, data pyld);
Graph *add_edge(Graph *gr, uint32_t src, uint32_t dst, double prb);
Graph *print_graph(Graph *gr);

#endif /* __GRAPH_H */
