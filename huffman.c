#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct _node
{
	char item;
	int priority;
	struct _node *next, *left, *right; 
} node;

typedef struct _priority_queue
{
	node *head;
} priority_queue;

int is_empty (priority_queue *queue)
{
	return (queue->head == NULL);
}

priority_queue* create_priority_queue ()
{
	 priority_queue *new_pq = (priority_queue*) malloc (sizeof (priority_queue));
	 return new_pq;
}

void enqueue (priority_queue *pq, char i, int p)
{
	node *new_node = (node*) malloc (sizeof (node));
	new_node->item = i;
	new_node->priority = p;
	if (is_empty(pq) || (p <= pq->head->priority))
	{
		
		new_node->next = pq->head;
		pq->head = new_node;
	}
	else
	{
		node *current = pq->head;
		while ((current->next != NULL) && (current->next->priority < new_node->priority))
		{
			current = current->next;
		}
		new_node->next = current->next;
		current->next = new_node;
	}
}

void print_queue (priority_queue *pq)
{
	node *aux = pq->head;
	while (aux != NULL)
	{
		printf ("%c ", aux->item);
		aux = aux->next;
	}
}

void print_pre_order(node *pq)
{	
	if (pq != NULL) {
		printf(" %c ", pq->item);
		print_pre_order(pq->left);
		print_pre_order(pq->right);
	}
}

priority_queue *add_sort (priority_queue *pq)
{
	node *current = pq->head->next;
	node *previous = pq->head;
	
	while ((current != NULL) && (pq->head->priority > current->priority))
	{
		previous = current;
		current = current->next;
	}
	if(pq->head != previous) {
		node *aux = pq->head;
		pq->head = pq->head->next;
		previous->next = aux;
		aux->next = current;	
	}

	return pq;
}

node* create_huffman_tree (priority_queue *pq)
{
	while (pq->head->next != NULL)
	{
		node *new_node = (node*) malloc (sizeof(node));
		new_node->item = '*';
		new_node->priority = pq->head->priority + pq->head->next->priority; 
		new_node->left = pq->head;
		new_node->right = pq->head->next;

		if(pq->head->next->next != NULL) 
		{
			node *aux = pq->head->next->next;
			pq->head->next->next = NULL;
			pq->head->next = NULL;
			new_node->next = aux;
			pq->head = new_node;

			pq = add_sort (pq);
		} else {
			pq->head = new_node;
		}
	}
}


int main ()
{
	int caracter = 0;
  	int array[256];
	FILE *arquivo = fopen("japao.jpg", "r");
	
	if (arquivo == NULL) {
		printf("File error\n");
		return 0;
	}

	for(int j = 0; j < 256; j++) {
		array[j] = 0;
	}

  	while(caracter != EOF) {
  		caracter = fgetc(arquivo);
  		array[caracter]++;

  	}

	int i, priority;
	char item;
	priority_queue *pq = create_priority_queue ();
	for(i = 0; i < 256; i++) {
		if(array[i] > 0) {
			enqueue(pq, i, array[i]);
		}
	} 
	create_huffman_tree(pq);
	print_pre_order (pq->head);

	return 0;
}
