#include "markov.h"

int main(int argc, char **argv) {

    /***
     * Redis test
     */

    puts("Redis test\n=====\n");

    redisContext *c;
    redisReply   *reply;

    const char *hostname = "127.0.0.1";
    int port = 6379;

    struct timeval timeout = {1, 500000 }; // 1.5 seconds

    c = redisConnectWithTimeout(hostname, port, timeout);
    if (c == NULL || c->err) {
        if (c) {
            printf("Connection error: %s\n", c->errstr);
            redisFree(c);
        }
        else {
            printf("Connection error: can't allocate redis context\n");
        }
        exit(1);
    }

    /* PING server */
    reply = redisCommand(c, "PING");
    printf("PING: %s\n", reply->str);
    freeReplyObject(reply);

    /* Set a key */
    reply = redisCommand(c, "SET %s %s",
            "foo", "hello world");
    printf("SET: %s\n", reply->str);
    freeReplyObject(reply);

    /***
     * Markov model test
     */

    puts("\nMarkov test\n=====\n");

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
    printf("\n\n");

    /* Serialize model */
    puts("Testing serialization\n");
    m = serialize_model(m, c, "test_model");

    /* Check serialization */
    reply = redisCommand(c, "HKEYS test_model");
    char *key;
    uint64_t edge, v1, v2;
    for (i = 0; i < reply->elements; i++) {
        switch (reply->element[i]->type) {
        case REDIS_REPLY_STRING:
            if (!strncmp(reply->element[i]->str, "size", sizeof("size"))) {
            }
            else {
                edge = (uint64_t) atol(reply->element[i]->str);
                v1 = edge >> 32;
                v2 = edge & 0xFFFFFFFF;

                printf("Read edge %d -> %lu\n", v1, v2);
            }

            break;
        default:
            printf("Error\n");
        }
    }
    freeReplyObject(reply);

    del_model(m);
    del_graph(gr);

    redisFree(c);
    return 0;
}

