all: geoquim

geoquim: C_Amostra.o C_SaidaDeDados.o C_Heteroatomica.o C_DBE.o C_ComponenteQuimico.o main.o
	g++ -o geoquim.exe C_Amostra.o C_SaidaDeDados.o C_Heteroatomica.o C_DBE.o C_ComponenteQuimico.o main.o

C_Amostra.o: C_Amostra.cpp
	g++ -c C_Amostra.cpp

C_SaidaDeDados.o: C_SaidaDeDados.cpp
	g++ -c C_SaidaDeDados.cpp

C_Heteroatomica.o: C_Heteroatomica.cpp
	g++ -c C_Heteroatomica.cpp

C_DBE.o: C_DBE.cpp
	g++ -c C_DBE.cpp

C_ComponenteQuimico.o : C_ComponenteQuimico.cpp
	g++ -c C_ComponenteQuimico.cpp

main.o: main.cpp
	g++ -c main.cpp

