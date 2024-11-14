#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

// Structure for storing a graph
struct Graph
{
    int vertexNum;
    int **edges;
};

// Structure to represent a node in A*
struct Node
{
    int f, g, h;
    int parent;
    bool is_in_closed_set;
};

// Creates a graph with V vertices
void createGraph(struct Graph *G, int V)
{
    G->vertexNum = V;
    G->edges = (int **)malloc(V * sizeof(int *));
    for (int i = 0; i < V; i++)
    {
        G->edges[i] = (int *)malloc(V * sizeof(int));
        for (int j = 0; j < V; j++) G->edges[i][j] = INT_MAX;
        G->edges[i][i] = 0;
    }
}

// Adds an edge to the graph with a specified weight
void addEdge(struct Graph *G, int src, int dst, int weight)
{
    G->edges[src][dst] = weight;
}

// Heuristic function (Manhattan distance for simplicity)
int heuristic(int start, int goal) { return abs(start - goal); }

// Utility function to find the node with the lowest f in the open set
int minFNode(int openSet[], struct Node nodes[], int openSetSize)
{
    int minF = INT_MAX;
    int minNode = -1;
    for (int i = 0; i < openSetSize; i++)
    {
        int node = openSet[i];
        if (nodes[node].f < minF)
        {
            minF = nodes[node].f;
            minNode = node;
        }
    }
    return minNode;
}

// Function to reconstruct and print the path from start to goal
void printPath(struct Node nodes[], int goal)
{
    if (nodes[goal].parent != -1)
    {
        printPath(nodes, nodes[goal].parent);
    }
    printf("%d ", goal);
}

// A* Algorithm function
void aStar(struct Graph *graph, int start, int goal)
{
    int V = graph->vertexNum;
    struct Node nodes[V];
    int openSet[V];
    int openSetSize = 0;

    // Initialize nodes and add start to open set
    for (int i = 0; i < V; i++)
    {
        nodes[i].f = nodes[i].g = nodes[i].h = INT_MAX;
        nodes[i].parent = -1;
        nodes[i].is_in_closed_set = false;
    }
    nodes[start].g = 0;
    nodes[start].h = heuristic(start, goal);
    nodes[start].f = nodes[start].g + nodes[start].h;
    openSet[openSetSize++] = start;

    // Main A* loop
    while (openSetSize > 0)
    {
        int current = minFNode(openSet, nodes, openSetSize);

        // If goal is reached, reconstruct path
        if (current == goal)
        {
            printf("Path from %d to %d: ", start, goal);
            printPath(nodes, goal);
            printf("\n");
            return;
        }

        // Remove current from open set
        int index = 0;
        while (index < openSetSize && openSet[index] != current) index++;
        for (int i = index; i < openSetSize - 1; i++)
            openSet[i] = openSet[i + 1];
        openSetSize--;

        nodes[current].is_in_closed_set = true;

        // Check neighbors
        for (int neighbor = 0; neighbor < V; neighbor++)
        {
            if (graph->edges[current][neighbor] != INT_MAX &&
                !nodes[neighbor].is_in_closed_set)
            {
                int tentative_g =
                    nodes[current].g + graph->edges[current][neighbor];

                if (tentative_g < nodes[neighbor].g)
                {
                    nodes[neighbor].parent = current;
                    nodes[neighbor].g = tentative_g;
                    nodes[neighbor].h = heuristic(neighbor, goal);
                    nodes[neighbor].f = nodes[neighbor].g + nodes[neighbor].h;

                    // Add neighbor to open set if not already present
                    bool inOpenSet = false;
                    for (int i = 0; i < openSetSize; i++)
                    {
                        if (openSet[i] == neighbor)
                        {
                            inOpenSet = true;
                            break;
                        }
                    }
                    if (!inOpenSet)
                        openSet[openSetSize++] = neighbor;
                }
            }
        }
    }

    printf("No path found from %d to %d\n", start, goal);
}

// Driver function
int main()
{
    int V, E;
    int src, dst, weight;
    struct Graph G;
    printf("Enter number of vertices: ");
    scanf("%d", &V);
    printf("Enter number of edges: ");
    scanf("%d", &E);
    createGraph(&G, V);

    for (int i = 0; i < E; i++)
    {
        printf("\nEdge %d \nEnter source: ", i + 1);
        scanf("%d", &src);
        printf("Enter destination: ");
        scanf("%d", &dst);
        printf("Enter weight: ");
        scanf("%d", &weight);
        addEdge(&G, src, dst, weight);
    }

    int start, goal;
    printf("\nEnter start node: ");
    scanf("%d", &start);
    printf("Enter goal node: ");
    scanf("%d", &goal);

    aStar(&G, start, goal);

    return 0;
}
