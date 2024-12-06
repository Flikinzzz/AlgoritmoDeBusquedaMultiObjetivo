#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

#define MAXNODES 4000000
#define MAXNEIGH 45
#define MAX_SOLUTIONS 1000000
#define MAX_RECYCLE   100000
#define LARGE  1000000000
#define BASE   10000000

#define max(x,y) ( (x) > (y) ? (x) : (y) )
#define min(x,y) ( (x) < (y) ? (x) : (y) )

// *********************************************** Main Data Structures *******************************************************
struct gnode;
typedef struct gnode gnode;

struct gnode {
    long long int id;
    unsigned h1;
    unsigned h2;
    unsigned long long int key;
    unsigned gmin;
    unsigned long heapindex;
};

struct snode;
typedef struct snode snode;

struct snode {
    int cost;
    int stops;
    struct snode* prev;
    int state;
    unsigned g1;
    unsigned g2;
    double key;
    unsigned long heapindex;
    snode *searchtree;
};

gnode* graph_node;
unsigned num_gnodes = 0;
unsigned adjacent_table[MAXNODES][MAXNEIGH] = {0};
unsigned goal, start;
snode* start_node;

unsigned long long int stat_expansions = 0;
unsigned long long int stat_generated = 0;
unsigned nsolutions = 0;

// *********************************************** Binary Heap for BOA* *******************************************************
#define HEAPSIZE 40000000
snode* heap[HEAPSIZE];
unsigned long int heapsize = 0;

void percolateup(int hole, snode* tmp) {
    if (heapsize != 0) {
        for (; hole > 1 && tmp->key < heap[hole / 2]->key; hole /= 2) {
            heap[hole] = heap[hole / 2];
            heap[hole]->heapindex = hole;
        }
        heap[hole] = tmp;
        heap[hole]->heapindex = hole;
    }
}

void percolateupordown(int hole, snode* tmp) {
    if (heapsize != 0) {
        if (hole > 1 && heap[hole / 2]->key > tmp->key)
            percolateup(hole, tmp);
    }
}

void insertheap(snode* thiscell) {
    if (thiscell->heapindex == 0)
        percolateup(++heapsize, thiscell);
    else
        percolateupordown(thiscell->heapindex, heap[thiscell->heapindex]);
}

snode* popheap() {
    snode* thiscell;
    if (heapsize == 0) return NULL;
    thiscell = heap[1];
    thiscell->heapindex = 0;
    --heapsize;
    return thiscell;
}

void emptyheap() {
	int i=1;
    for (i = 1; i <= heapsize; ++i)
        heap[i]->heapindex = 0;
    heapsize = 0;
}

snode* new_node() {
    snode* state = (snode*)malloc(sizeof(snode));
    state->heapindex = 0;
    return state;
}

snode* topheap() {
    if (heapsize == 0) return NULL;
    return heap[1];
}

// *********************************************** BOA* Implementation *******************************************************
void boastar(int instance_id) {
    emptyheap();
    start_node = new_node();
    start_node->state = start;
    start_node->g1 = 0;
    start_node->g2 = 0;
    start_node->key = 0;
    start_node->searchtree = NULL;
    insertheap(start_node);

    stat_expansions = 0;
    stat_generated = 0;
    nsolutions = 0;

    struct timeval tstart, tend;
    gettimeofday(&tstart, NULL);

    while (topheap() != NULL) {
        snode* n = popheap();
        if (n->state == goal) {
            nsolutions++;
            continue;
        }
        int d=1;
		for (d = 1; d < adjacent_table[n->state][0] * 3; d += 3) {
            snode* succ = new_node();
            unsigned nsucc = adjacent_table[n->state][d];
            unsigned cost1 = adjacent_table[n->state][d + 1];
            unsigned cost2 = adjacent_table[n->state][d + 2];
            unsigned newg1 = n->g1 + cost1;
            unsigned newg2 = n->g2 + cost2;

            succ->state = nsucc;
            succ->g1 = newg1;
            succ->g2 = newg2;
            succ->key = newg1 * BASE + newg2;
            succ->searchtree = n;
            insertheap(succ);
            stat_generated++;
        }
        stat_expansions++;
    }

    gettimeofday(&tend, NULL);

    double elapsed_time = (tend.tv_sec - tstart.tv_sec) +
                          (tend.tv_usec - tstart.tv_usec) / 1000000.0;

    // Imprimir resultados en el formato solicitado
    printf("Instance %d: start=%d, goal=%d; Solutions=%d; Time=%.6f; Expansions=%llu; Generated=%llu\n",
           instance_id,       // #instancia
           start, goal,       // start -> goal
           nsolutions,        // número de soluciones
           elapsed_time,      // tiempo de ejecución
           stat_expansions,   // #nodos expandidos
           stat_generated);   // #nodos generados
}

// *********************************************** Reading the Graph *******************************************************
void read_graph(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Error: Cannot open file %s\n", filename);
        exit(1);
    }
    printf("Reading graph from file: %s\n", filename);
    int start, stop1, stop2, goal;
    while (fscanf(file, "%d %d %d %d", &start, &stop1, &stop2, &goal) != EOF) {
        adjacent_table[start][++adjacent_table[start][0]] = stop1;
        adjacent_table[stop1][++adjacent_table[stop1][0]] = stop2;
        adjacent_table[stop2][++adjacent_table[stop2][0]] = goal;
        num_gnodes = max(num_gnodes, goal + 1);
    }
    fclose(file);
    printf("Graph reading complete.\n");
}

// *********************************************** Main Function *******************************************************
int main() {
    const char* graph_file = "NY-queries-2p.txt";
    read_graph(graph_file);

    int instance_id = 1;
    FILE* file = fopen(graph_file, "r");

    if (!file) {
        printf("Error: Cannot open file %s\n", graph_file);
        exit(1);
    }

    int s, p1, p2, g;
    while (fscanf(file, "%d %d %d %d", &s, &p1, &p2, &g) != EOF) {
        start = s;
        goal = p1;
        boastar(instance_id++);

        start = p1;
        goal = p2;
        boastar(instance_id++);

        start = p2;
        goal = g;
        boastar(instance_id++);
    }

    fclose(file);
    return 0;
}

