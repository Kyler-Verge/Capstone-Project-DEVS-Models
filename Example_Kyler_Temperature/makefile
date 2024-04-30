all: main.o
	g++ -g -o Blinky main.o 

main.o: main.cpp DEVS_Models/
	g++ -g -c -std=c++17 -I ../../../include/cadmium/ -I DEVS_Models main.cpp -o main.o

clean:
	rm -f *.o
	rm -f *.csv
	rm -f Blinky

