all : main

main : main.o libhuffmancompress.a
	gcc -Wall main.o libhuffmancompress.a -o main

main.o : main.c
	gcc -c main.c
	
libhuffmancompress.a : encode.o decode.o
	ar -r libhuffmancompress.a encode.o decode.o

encode.o : encode.c
	gcc -c encode.c
	
decode.o : decode.c
	gcc -c decode.c

clean :
	rm -rf *.o main
