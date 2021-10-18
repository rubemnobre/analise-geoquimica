all: geoquim

geoquim: input_data.o main.o
	g++ -o geoquim.exe input_data.o main.o

input_data.o: input_data.cpp input_data.hpp
	g++ -c input_data.cpp

main.o: main.cpp input_data.cpp input_data.hpp
	g++ -c main.cpp

