all : main

main : main.o encode.o decode.o
	gcc -Wall main.o encode.o decode.o -o main

main.o : main.c
	gcc -c main.c

encode.o : encode.c
	gcc -c encode.c
	
decode.o : decode.c
	gcc -c decode.c

clean :
	rm -rf *.o main