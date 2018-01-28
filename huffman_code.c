#include <stdio.h>
#include <stdlib.h>
#include "huffman_code.h"
#include <string.h>

#define MAX_TREE_HEIGHT 1000


/* Crée un nouveau noeud */
struct NodeMinHeap* newNode(char data, unsigned frequency)
{
	struct NodeMinHeap* tmp = (struct NodeMinHeap*) malloc(sizeof(struct NodeMinHeap));

	tmp->left = NULL;
	tmp->right = NULL;
	tmp->frequency = frequency;
	tmp->data = data;

	return tmp;
}

/* Crée un nouveau MinHeap de capacité définie */
struct MinHeap* createMinHeap(unsigned capacity)
{
	struct MinHeap* minHeap = (struct MinHeap*) malloc(sizeof(struct MinHeap));

	minHeap->size = 0;
	minHeap->capacity = capacity;
	minHeap->array = (struct NodeMinHeap**) malloc(minHeap->capacity * sizeof(struct NodeMinHeap*));
	return minHeap;
}

/* Échange deux noeuds */
void swapMinHeapNodes(struct NodeMinHeap** a, struct NodeMinHeap** b)
{
	struct NodeMinHeap* t = *a;
	*a = *b;
	*b = t;
}

/* MinHeapify le MinHeap */
void minHeapify(struct MinHeap* minHeap, int idx)
{
	int smallest = idx;
	int left = 2 * idx + 1;
	int right = 2 * idx + 2;

	if(left < minHeap->size && minHeap->array[left]->frequency < minHeap->array[smallest]->frequency)
		smallest=left;

	if(right < minHeap->size && minHeap->array[right]->frequency < minHeap->array[smallest]->frequency)
		smallest=right;

	if (smallest != idx) {
		swapMinHeapNodes(&minHeap->array[smallest], &minHeap->array[idx]);
		minHeapify(minHeap, smallest);
	}
}

/* Dit si le MinHeap n'est composé que d'une racine */
int isSizeOne(struct MinHeap* minHeap)
{
	return (minHeap->size == 1);
}

/* Retourne le noeud de valeur minimum */
struct NodeMinHeap* getMin(struct MinHeap* minHeap)
{
	struct NodeMinHeap* tmp = minHeap->array[0];
	minHeap->array[0] = minHeap->array[minHeap->size - 1];

	--minHeap->size;
	minHeapify(minHeap, 0);

	return tmp;
}

/* Insert un nouveau noeud dans un MinHeap */
void insertMinHeap(struct MinHeap* minHeap, struct NodeMinHeap* nodeMinHeap)
{
	++minHeap->size;
	int i = minHeap->size - 1;

	while (i && nodeMinHeap->frequency < minHeap->array[(i - 1) / 2]->frequency) {
		minHeap->array[i] = minHeap->array[(i - 1) / 2];
		i = (i - 1) /2;
	}

	minHeap->array[i] = nodeMinHeap;
}

/* Fabrique un MinHeap en le MinHeapifiant */
void buildMinHeap(struct MinHeap* minHeap)
{
	int n = minHeap->size - 1;
	for(int i = (n-1)/2; i >= 0; --i)
		minHeapify(minHeap, i);
}

/* Affiche un tableau d'int de taille n */
void printArray(int arr[], int n)
{
	for(int i = 0; i < n; ++i)
		printf("%d", arr[i]);

	printf("\n");
}

/* Permet de savoir si un noeud est une feuille */
int nodeIsLeaf(struct NodeMinHeap* root)
{
	return !(root->left) && !(root->right);
}

/* Crée un MinHeap et y insert les valeurs contenu dans data */
struct MinHeap* createAndBuildMinHeap(char data[], unsigned int frequency[], int size)
{
	struct MinHeap* minHeap = createMinHeap(size);

	for(int i = 0; i < size; ++i)
		minHeap->array[i] = newNode(data[i], frequency[i]);

	minHeap->size = size;
	buildMinHeap(minHeap);

	return minHeap;
}

/* Crée un arbre d'Huffman à partir d'un minHeap créé selon les valeurs de data[] et frequency[] */
struct NodeMinHeap* buildHuffmanTree(char data[], unsigned int frequency[], int size)
{
	struct NodeMinHeap *left, *right, *top;

	struct MinHeap* minHeap = createAndBuildMinHeap(data, frequency, size);

	while(!isSizeOne(minHeap)) {
		left = getMin(minHeap);
		right = getMin(minHeap);

		top = newNode('#', left->frequency + right->frequency);

		top->left = left;
		top->right = right;

		insertMinHeap(minHeap, top);
	}

	return getMin(minHeap);
}

/* Traduit l'arbre d'Huffman en un dictionaire */
void createDict(struct NodeMinHeap* root, int arr[], int top, int** result, unsigned int displayTree)
{
	if(root->left) {
		arr[top] = 0;
		createDict(root->left, arr, top+1, result, displayTree);
	}

	if(root->right) {
		arr[top] = 1;
		createDict(root->right, arr, top+1, result, displayTree);
	}

	if(nodeIsLeaf(root)) {
		if(displayTree){
			printf("%c = ", root->data); //affiche le charactère
			printArray(arr, top); //affiche le tableau d'entiers des bits correspondant
		}
		result[root->data] = (int*) malloc(sizeof(int) * (top + 2));

		//sauvegarde des valeurs dans result[charactere] qui servira de dictionnaire
		//le tableau d'int a la forme "charactere","nombre de bits","bit1","bit2",etc.
		result[root->data][0] = root->data;
		result[root->data][1] = top;
		for(int i = 0; i < top; i++){
			result[root->data][i+2] = arr[i];
		}
	}
}

/* Effectue tout le traitement */
int ** HuffmanCodes(char data[], unsigned int frequency[], int size, unsigned int displayTree)
{
	struct NodeMinHeap* root = buildHuffmanTree(data, frequency, size);

	int arr[MAX_TREE_HEIGHT],top = 0;

	int **result = (int**) malloc(MAX_TREE_HEIGHT*sizeof(int*));

	if(displayTree){
		printf("\nVoici l'arbre d'Huffman :\n\n");
	}
	createDict(root, arr, top, result, displayTree);
	printf("\n");
	return result;
}
