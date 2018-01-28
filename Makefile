all : final clean

final: huffman_code.o huffman_prog.o
	gcc huffman_code.o huffman_prog.o -o huffman

huffman_prog.o: huffman_prog.c
	gcc -c huffman_prog.c -o huffman_prog.o

huffman_code.o: huffman_code.c
	gcc -c huffman_code.c -o huffman_code.o

clean:
	rm -rf *.o

clear: clean
	rm -rf huffman
