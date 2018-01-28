void countOccur(char* filename, int* occur);

void compressFile(char* filenameRead, char* filenameWrite, unsigned int displayTree);

void uncompressFile(char * filenameRead, char * filenameWrite, unsigned int displayResult);

char * int_array_to_string(int arr[], int size_of_array);

struct Charactere * getCharactere(char charactere, int ** array);

char * decode(struct Charactere ** characteres, int characteresSize, char * string);

char * getSetOfCharInFile(char * filename, int * size, unsigned int * frequency);
