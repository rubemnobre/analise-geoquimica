all: geoquim

geoquim: input_data.o data_analysis.o main.o
	g++ -o geoquim.exe data_analysis.o input_data.o main.o

input_data.o: input_data.cpp input_data.hpp
	g++ -c input_data.cpp

data_analysis.o: data_analysis.cpp input_data.hpp
	g++ -c data_analysis.cpp

main.o: main.cpp input_data.cpp data_analysis.cpp input_data.hpp
	g++ -c main.cpp

