all: hh_sim

hh_sim: main.o struct.o sin.o Mad.o Add.o mul.o
	gcc -o hh_sim main.o struct.o sin.o Mad.o Add.o mul.o -lm

main.o: main.c struct.h sin.h Mad.h Add.h mul.h
	gcc -c main.c

struct.o: struct.c struct.h
	gcc -c struct.c

sin.o: sin.c sin.h struct.h Mad.h Add.h mul.h
	gcc -c sin.c

Mad.o: Mad.c Mad.h struct.h Add.h mul.h
	gcc -c Mad.c

Add.o: Add.c Add.h struct.h
	gcc -c Add.c

mul.o: mul.c mul.h struct.h
	gcc -c mul.c

clean:
	rm -f *.o hh_sim
