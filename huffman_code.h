struct Charactere {
    char * value;
    char * code;
    int length;
};

/* Définition d'un noeud de MinHeap */
struct NodeMinHeap {
        char data;
        unsigned frequency;
        struct NodeMinHeap *left, *right;
};

/* Définition d'un MinHeap */
struct MinHeap {
        unsigned size;
        unsigned capacity;
        struct NodeMinHeap** array;
};

struct NodeMinHeap* newNode(char data, unsigned frequency);

struct MinHeap* createMinHeap(unsigned capacity);

void swapMinHeapNodes(struct NodeMinHeap** a, struct NodeMinHeap** b);

void minHeapify(struct MinHeap* minHeap, int idx);

int isSizeOne(struct MinHeap* minHeap);

struct NodeMinHeap* getMin(struct MinHeap* minHeap);

void insertMinHeap(struct MinHeap* minHeap, struct NodeMinHeap* nodeMinHeap);

void buildMinHeap(struct MinHeap* minHeap);

void printArray(int arr[], int n);

int nodeIsLead(struct NodeMinHeap* root);

struct MinHeap* createAndBuildMinHeap(char data[], unsigned int frequency[], int size);

struct NodeMinHeap* buildHuffmanTree(char data[], unsigned int frequency[], int size);

void createDict(struct NodeMinHeap* root, int arr[], int top, int** result, unsigned int displayTree);

int ** HuffmanCodes(char data[], unsigned int frequency[], int size, unsigned int displayTree);
