all: main

main: main.o halaman_web.o
	gcc -o main main.o halaman_web.o

main.o: main.c halaman_web.h
	gcc -c main.c

halaman_web.o: halaman_web.c halaman_web.h
	gcc -c halaman_web.c

clean:
	rm -f *.o main