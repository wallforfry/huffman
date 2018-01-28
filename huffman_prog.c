#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "huffman_code.h"
#include "huffman_prog.h"


int main(int argc, char** argv)
{
		int choice;

		while(1){
			printf("\nCompression par algorithme d'Huffman :\n\n");
      printf("\t 1) Compresser un fichier\n");
      printf("\t 2) Décompresser un fichier\n");
			printf("\t 3) Démonstration compression puis décompression\n");
			printf("\t 4) Informations\n");
      printf("\t 0) Quitter\n");
			scanf("%d", &choice);

			switch (choice) {
				default:
				case 0:
					return 0;
				case 1:
					printf("Quel est le fichier à compresser ?\n");
					char fileToCompress[256];
					scanf("%s", fileToCompress);

					printf("Quel est le nom du fichier de sortie ?\n");
					char fileCompressed[256];
					scanf("%s", fileCompressed);

					printf("Voulez-vous afficher le dictionnaire d'Huffman créé ? o/N\n");
					char displayTree[10];
					scanf("%s", displayTree);
					if(displayTree[0] == 'o')
						compressFile(fileToCompress, fileCompressed, 1);
					else
						compressFile(fileToCompress, fileCompressed, 0);

					printf("Le fichier a bien été compressé\n");

					break;
				case 2:
					printf("Quel est le fichier à décompresser ?\n");
					char fileToDeCompress[256];
					scanf("%s", fileToDeCompress);

					printf("Quel est le nom du fichier de sortie ?\n");
					char fileDeCompressed[256];
					scanf("%s", fileDeCompressed);

					printf("Voulez-vous afficher le texte décompressé ? o/N\n");
					char displayText[10];
					scanf("%s", displayText);
					if(displayText[0] == 'o')
						uncompressFile(fileToDeCompress, fileDeCompressed, 1);
					else
						uncompressFile(fileToDeCompress, fileDeCompressed, 0);


					printf("Le fichier a bien été décompressé\n");

					break;
				case 3:
					printf("\n\nDémonstration de compression du fichier file.txt vers output.txt\n\n");

					printf("Début de la compression..\n\n");
					compressFile("file.txt", "output.txt", 1);

					printf("\n\nDébut de la décompression..\n\n");
					uncompressFile("output.txt", "decompressed.txt", 1);

					printf("\n\nFin de la démonstration\n\n");
					break;
				case 4:
					printf("\n\nProjet réalisé par Wallerand DELEVACQ, Johana COHEN et Clément BARBIÉ\n");
					break;
			}
			printf("\n\n\nRetour au menu..\n\n\n");
		}
        return 0;
}

/*
 * Compresse le fichier "filenameRead" dans le fichier "filenameWrite"
 * displayTree => afficher ou non l'arbre à la compression
 */
void compressFile(char* filenameRead, char* filenameWrite, unsigned int displayTree){
	FILE* openf = fopen(filenameRead, "r");
	FILE* writef = fopen(filenameWrite, "w");

	char separator = ';';

	int count = 0;
	unsigned int * frequency = (unsigned int*) malloc(256 * sizeof(unsigned int));

	char * arr = getSetOfCharInFile(filenameRead, &count, frequency);

	int ** huffmanDict = HuffmanCodes(arr, frequency, count, displayTree);

	//Write dict
	for(int i = 0; i < count; i++){

			struct Charactere * encodedChar = getCharactere(arr[i], huffmanDict);
			fwrite(encodedChar->value, encodedChar->length, 1, writef);
			fwrite(&(arr[i]), 1, 1, writef);
	}

	//Write separator
	fwrite(&separator, sizeof(separator), 1, writef);

	//Write data
	int notFinished = 1;
	char c;
	while(notFinished){
		notFinished = fread(&c, sizeof(char), 1, openf);
		if(notFinished){
			struct Charactere * encodedChar = getCharactere(c, huffmanDict);
			fwrite(encodedChar->value, encodedChar->length, 1, writef);
		}
	}

	fclose(openf);
	fclose(writef);
}

/*
 * Décompresse le fichier "filenameRead" dans "filenameWrite"
 */
void uncompressFile(char * filenameRead, char * filenameWrite, unsigned int displayResult){

	struct Charactere * decodingDict[256];
	int position = 0;

	FILE * f = fopen(filenameRead, "r");
	int notFinished = 1;
	char c;

	struct Charactere * charactere = (struct Charactere *) malloc(sizeof(struct Charactere));
	int length = 0;
	char * code = (char*) malloc(sizeof(char)*100);

	int headerSize = 0;

	fseek(f, 0, SEEK_END);
	int fileSize = ftell(f);
	rewind(f);

	char * buffer = malloc((fileSize + 1) * sizeof(char));


	while (notFinished) {
		notFinished = fread(&c, sizeof(char), 1, f);
		if(c == ';'){
			//traitement du text
			headerSize++;
			notFinished = 0;
			fread(buffer, fileSize, 1, f);
			buffer[fileSize - headerSize] = '\0';
		}
		if(notFinished){
			//traitement du dictionnaire
			if(c != '0' && c != '1'){
				//récupération du charactere
				code[length] = '\0';
				charactere->value = c;
				charactere->length = length;
				charactere->code = code;
				decodingDict[position] = charactere;

				position++;
				charactere = (struct Charactere *) malloc(sizeof(struct Charactere));
				length = 0;
				code = (char*) malloc(sizeof(char) *100);
			}
			else{
					if(c == '1')
						code[length] = '1';
					if(c == '0')
						code[length] = '0';
					length++;
			}
			headerSize++;
		}
	}
	fclose(f);

	//decodage depuis le dictionnaire
	//printf("nombre de characteres : %d / fileSize : %d\n", position + 1, fileSize);
	char * result = decode(decodingDict, position, buffer);

	if(displayResult){
		printf("Texte décompressé : %s\n", result);
	}

	//écriture du fichier décompressé
	FILE * decompressedFile = fopen(filenameWrite, "w");
	fputs(result, decompressedFile);
	fclose(decompressedFile);

}

/*
 * Renvoi un set des charactere contenus dans le fichier ainsi qu'un tableau de leur fréquences
 */
char * getSetOfCharInFile(char * filename, int * size, unsigned int * frequency) {
		int count = 0;
		char * set = (char *) malloc(sizeof(char) * 256);
		int position[256];
		for(int i = 0; i < 256; i++){
			position[i] = -1;
		}

		FILE* f = fopen(filename, "r");
		int notFinished = 1;
		char c;

		while(notFinished){
			notFinished = fread(&c, sizeof(char), 1, f);
			if(notFinished){
				if(position[c] == -1){
					position[c] = count;
					set[position[c]] = c;
					count++;
				}
				frequency[position[c]] += 1;
			}
		}
		fclose(f);
		*size = count;

		return set;
		char * finalSet = (char *) malloc(sizeof(char) * count);
		for(int i = 0; i < count; i++){
			finalSet[i] = set[i];
		}


		free(set);
		return finalSet;

}

/*
 * Décode la string "string" à partir du dictionnaire "characteres"
 */
char * decode(struct Charactere ** characteres, int characteresSize, char * string){
				char * result = (char*) calloc(strlen(string), sizeof(char));

				int position = 0;

				int cptr = 0;

				//Affichage d'info sur la string et le dictionnaire
				/*
				printf("String : %s  Length : %d\n", string, strlen(string));
				printf("Dict Length : %d \n", characteresSize);
				//*/
				//Pour chaque charactere de string
				while(string[cptr] != '\0'){
					int isOk = 1;
					struct Charactere * charactere = NULL;
					//On parcour le dictionnaire
					for(int i = 0; i < characteresSize; i++){
							charactere = characteres[i];
							isOk = 1;
							int tmp = 0;

							//On test chaque charactere de la string comparé au dictionnaire
							for(int j = 0; j< charactere->length; j++){
								if(string[cptr + tmp] != charactere->code[j]){
									isOk = 0;
									charactere = NULL;
									break;
								}
								else {
									tmp++;
								}
							}
							if(isOk){
								cptr += charactere->length - 1;
								break;
							}
					}
					//On a le bon charactere
					if(isOk){
						result[position] = charactere->value;
						position++;
					}

					cptr++;
				}
				return result;
}

/*
 * Renvoit une struct Charactere associé au charactere "charactere" grâce au dictionaire "array"
 */
struct Charactere * getCharactere(char charactere, int ** array){
		struct Charactere * result = (struct Charactere*) malloc(sizeof(struct Charactere));
		result->value = int_array_to_string(array[charactere], array[charactere][1]);
		result->length = strlen((result->value));
		return result;
}

/*
 * Convertit un tableau d'int en String
 */
char * int_array_to_string(int arr[], int size_of_array) {
	char * returnstring = (char*) malloc(size_of_array * (sizeof(char) + 1));
  for (int i = 2; i < size_of_array + 2; i++){
  	if(arr[i] == 1)
			returnstring[i-2] = '1';
		if(arr[i] == 0)
			returnstring[i-2] = '0';
	}
	returnstring[size_of_array] = '\0';
  return returnstring;
}
