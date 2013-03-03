
hw1: hw1.c misc.o
	gcc -Wall -o hw1 $< misc.o
misc.o: misc.c misc.h
	gcc -c -o misc.o $<
run: hw1
	./hw1
clean:
	rm -f ./hw1 ./misc.o
rebuild: clean hw1




