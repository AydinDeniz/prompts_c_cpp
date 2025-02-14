#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Structure to represent a node in an adjacency list
typedef struct AdjListNode {
    int dest;
    int weight;
    struct AdjListNode* next;
} AdjListNode;

// Structure to represent an adjacency list
typedef struct AdjList {
    AdjListNode* head;
} AdjList;

// Structure to represent a graph, a graph is an array of adjacency lists.
// Size of array will be V (number of vertices in graph)
typedef struct Graph {
    int V;
    struct AdjList* array;
} Graph;

// Function to create a new adjacency list node
AdjListNode* newAdjListNode(int dest, int weight) {
    AdjListNode* newNode = (AdjListNode*)malloc(sizeof(AdjListNode));
    newNode->dest = dest;
    newNode->weight = weight;
    newNode->next = NULL;
    return newNode;
}

// Function to create a graph of V vertices
Graph* createGraph(int V) {
    Graph* graph = (Graph*)malloc(sizeof(Graph));
    graph->V = V;
    graph->array = (AdjList*)malloc(V * sizeof(AdjList));

    for (int i = 0; i < V; ++i)
        graph->array[i].head = NULL;

    return graph;
}

// Function to add an edge to an undirected graph
void addEdge(Graph* graph, int src, int dest, int weight) {
    AdjListNode* newNode = newAdjListNode(dest, weight);
    newNode->next = graph->array[src].head;
    graph->array[src].head = newNode;

    // Since graph is undirected, add an edge from dest to src also
    newNode = newAdjListNode(src, weight);
    newNode->next = graph->array[dest].head;
    graph->array[dest].head = newNode;
}

// Utility function to print the adjacency list representation of graph
void printGraph(Graph* graph) {
    for (int v = 0; v < graph->V; ++v) {
        AdjListNode* pCrawl = graph->array[v].head;
        printf("\n Adjacency list of vertex %d\n head ", v);
        while (pCrawl) {
            printf("-> %d(%d) ", pCrawl->dest, pCrawl->weight);
            pCrawl = pCrawl->next;
        }
        printf("\n");
    }
}

// Helper function for DFS
void DFSUtil(Graph* graph, int v, bool visited[]) {
    visited[v] = true;
    printf("%d ", v);

    for (AdjListNode* pCrawl = graph->array[v].head; pCrawl != NULL; pCrawl = pCrawl->next) {
        if (!visited[pCrawl->dest]) {
            DFSUtil(graph, pCrawl->dest, visited);
        }
    }
}

// Function to perform DFS traversal
void DFS(Graph* graph, int startVertex) {
    bool* visited = (bool*)malloc(graph->V * sizeof(bool));
    for (int i = 0; i < graph->V; i++) {
        visited[i] = false;
    }
    printf("DFS starting from vertex %d:\n", startVertex);
    DFSUtil(graph, startVertex, visited);
    free(visited);
    printf("\n");
}

// Function to perform BFS traversal
void BFS(Graph* graph, int startVertex) {
    bool* visited = (bool*)malloc(graph->V * sizeof(bool));
    for (int i = 0; i < graph->V; i++) {
        visited[i] = false;
    }

    int* queue = (int*)malloc(graph->V * sizeof(int));
    int front = -1;
    int rear = -1;

    visited[startVertex] = true;
    queue[++rear] = startVertex;

    printf("BFS starting from vertex %d:\n", startVertex);

    while (front != rear) {
        int currentVertex = queue[++front];
        printf("%d ", currentVertex);

        for (AdjListNode* pCrawl = graph->array[currentVertex].head; pCrawl != NULL; pCrawl = pCrawl->next) {
            int adjVertex = pCrawl->dest;
            if (!visited[adjVertex]) {
                queue[++rear] = adjVertex;
                visited[adjVertex] = true;
            }
        }
    }
    free(queue);
    free(visited);
    printf("\n");
}

int main() {
    int V = 5;
    Graph* graph = createGraph(V);
    addEdge(graph, 0, 1, 2);
    addEdge(graph, 0, 4, 1);
    addEdge(graph, 1, 2, 4);
    addEdge(graph, 1, 3, 3);
    addEdge(graph, 1, 4, 2);
    addEdge(graph, 2, 3, 5);
    addEdge(graph, 3, 4, 1);

    printGraph(graph);

    DFS(graph, 0);
    BFS(graph, 0);

    return 0;
}