all: geoquim

geoquim: C_Amostra.o C_SaidaDeDados.o C_Heteroatomica.o C_DBE.o C_ComponenteQuimico.o C_EntradaDeDados.o C_DistC.o C_Grafico.o main.o C_SimuladorTratamentoDados_FT_ICRMS.o
	g++ -o geoquim.exe C_Amostra.o C_SaidaDeDados.o C_Heteroatomica.o C_DBE.o C_ComponenteQuimico.o C_EntradaDeDados.o C_DistC.o C_Grafico.o main.o C_SimuladorTratamentoDados_FT_ICRMS.o

C_SimuladorTratamentoDados_FT_ICRMS.o: C_SimuladorTratamentoDados_FT_ICRMS.cpp C_SimuladorTratamentoDados_FT_ICRMS.hpp
	g++ -c C_SimuladorTratamentoDados_FT_ICRMS.cpp

C_Grafico.o: C_Grafico.cpp C_Grafico.hpp
	g++ -c C_Grafico.cpp

C_EntradaDeDados.o: C_EntradaDeDados.cpp C_EntradaDeDados.hpp
	g++ -c C_EntradaDeDados.cpp

C_DistC.o: C_DistC.cpp C_DistC.hpp
	g++ -c C_DistC.cpp

C_Amostra.o: C_Amostra.cpp C_Amostra.hpp
	g++ -c C_Amostra.cpp

C_SaidaDeDados.o: C_SaidaDeDados.cpp C_SaidaDeDados.hpp
	g++ -c C_SaidaDeDados.cpp

C_Heteroatomica.o: C_Heteroatomica.cpp C_Heteroatomica.hpp
	g++ -c C_Heteroatomica.cpp

C_DBE.o: C_DBE.cpp C_DBE.hpp
	g++ -c C_DBE.cpp

C_ComponenteQuimico.o : C_ComponenteQuimico.cpp C_ComponenteQuimico.hpp
	g++ -c C_ComponenteQuimico.cpp

main.o: main.cpp
	g++ -c main.cpp

