#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

struct node {
	int id;
	struct node *next;
};

struct graph {
	int size;
	struct node **nodes;
};

struct queue {
	struct node *head;
	struct node *tail;
};

void push(struct queue *q, int id)
{
	struct node *np = malloc(sizeof(struct node));
	np->id = id;
	if (q->head)
		q->tail->next = np;
	else
		q->head = np;
	q->tail = np;
}

int pop(struct queue *q)
{
	assert(q->head);
	struct node *np = q->head;
	int id = np->id;
	if (q->head == q->tail)
		q->head = q->tail = NULL;
	else
		q->head = q->head->next;
	free(np);
	return id;
}

int empty(const struct queue *q)
{
	return q->head == NULL;
}

void bfs(const struct graph *g, int id)
{
	struct queue q = {};
	int *v = malloc(g->size * sizeof(int));
	for (int i = 0; i < g->size; ++i)
		v[i] = 0;

	v[id] = 1;
	push(&q, id);

	while (!empty(&q)) {
		id = pop(&q);
		printf("at node: %d\n", id);
		struct node *np = g->nodes[id];
		while (np) {
			if (!v[np->id]) {
				v[np->id] = 1;
				push(&q, np->id);
			}
			np = np->next;
		}
	}

	free(v);
}

struct graph *new_graph(int n)
{
	struct graph *g = malloc(sizeof(struct graph));
	g->size = n;
	g->nodes = malloc(n * sizeof(struct node *));
	for (int i = 0; i < n; ++i)
		g->nodes[i] = NULL;
	return g;
}

void free_list(struct node *np)
{
	struct node *tmp;
	while (np) {
		tmp = np->next;
		free(np);
		np = tmp;
	}
}

void free_graph(struct graph *g)
{
	for (int i = 0; i < g->size; ++i)
		free_list(g->nodes[i]);
	free(g->nodes);
	free(g);
}

void push_node(struct node **head, int v)
{
	struct node *np = malloc(sizeof(struct node));
	np->id = v;
	np->next = *head;
	*head = np;
}

void add_edge(struct graph *g, int v, int u)
{
	push_node(&g->nodes[v], u);
	push_node(&g->nodes[u], v);
}

int main()
{
	struct graph *g = new_graph(8);

	add_edge(g, 0, 1);
	add_edge(g, 1, 2);
	add_edge(g, 4, 3);
	add_edge(g, 1, 4);
	add_edge(g, 3, 7);
	add_edge(g, 4, 7);
	add_edge(g, 5, 6);
	add_edge(g, 1, 5);

	bfs(g, 0);
}
