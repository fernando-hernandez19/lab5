

#include <stdlib.h>
#include <string.h>
#include "graph.h"
#include "list.h"
#include "map.h"
// Se asume la inclusión de Map.h y List.h

/* =========================================
 *         ESTRUCTURAS INTERNAS
 * ========================================= */

struct Graph {
    // Un solo mapa basta: Llave (char* label) -> Valor (List* de Edge*)
    Map* adjacencyMap; 
};

// Función auxiliar para comparar strings en el mapa
int is_equal_string(void *key1, void *key2) {
    return strcmp((char*)key1, (char*)key2) == 0;
}

/* =========================================
 *         IMPLEMENTACIÓN
 * ========================================= */

Graph* createGraph() {
    Graph* graph = (Graph*) malloc(sizeof(Graph));
    if (graph == NULL) return NULL;

    graph->adjacencyMap = map_create(is_equal_string);
    if(graph->adjacencyMap == NULL) {
        free(graph);
    }
    return graph;
}

void addNode(Graph* g, const char* label) {
    if (!g || !label) return;
    
    if (map_search(g->adjacencyMap, (void*)label) != NULL) return;
    
    char* labelCopy = strdup(label);
    if (!labelCopy) return;
    
    List* edgesList = list_create();
    if (!edgesList) {
        free(labelCopy);
        return;
    }
    map_insert(g->adjacencyMap, labelCopy, edgesList);
    

}

void addEdge(Graph* g, const char* src, const char* dest, int weight) {
    if (!g || !src || !dest) return;
    
    char* destCopy = strdup(dest);
    if (!destCopy) return;
    
    Edge* newEdge = (Edge*) malloc(sizeof(Edge));
    if (!newEdge) {
        free(destCopy);
        return;
    }
    newEdge->target = destCopy;
    newEdge->weight = weight;
    
    MapPair* pair = map_search(g->adjacencyMap, (void*)src);
    if (pair == NULL) {
        free(newEdge->target);
        free(newEdge);
        return;
    }
    List* edgesList = (List*)pair->value;
    list_pushBack(edgesList, newEdge);

    

}

List* getEdges(Graph* g, const char* label) {
    if (!g || !label) return NULL;
    MapPair* pair = map_search(g->adjacencyMap, (void*)label);
    if (pair != NULL)
        return (List*)pair->value;
    
    return NULL;
}

int getWeight(Graph* g, const char* label1, const char* label2) {
    if (!g || !label1 || !label2) return -1;
    
    MapPair* pair = map_search(g->adjacencyMap, (void*)label1);
    if (pair == NULL) return -1;
    List* edgesList = (List*)pair->value;
    
    Edge* e = (Edge*)list_first(edgesList);
    while (e != NULL)
         if (strcmp(e->target, label2) == 0)
             return e->weight;
         else
             e = (Edge*)list_next(edgesList);

    
    return -1; 
}


List* getAdjacentLabels(Graph* g, const char* label) {
    if (!g || !label) return NULL;
    
    MapPair* pair = map_search(g->adjacencyMap, (void*)label);
    if (pair != NULL)
        return (List*)pair->value;
     
    return NULL; 
}

void destroyGraph(Graph* g) {
    if (!g) return;

    MapPair* pair = map_first(g->adjacencyMap);
    while (pair != NULL) {
        char* label = (char*)pair->key;
        List* edgesList = (List*)pair->value;

        
        Edge* e = (Edge*)list_first(edgesList);
        while (e != NULL) {
            free(e->target); 
            free(e);         
            e = (Edge*)list_next(edgesList);
        }

        list_clean(edgesList);
        free(edgesList);

        free(label);

        pair = map_next(g->adjacencyMap);
    }

    
    map_clean(g->adjacencyMap);
    free(g->adjacencyMap);
    free(g);
}
