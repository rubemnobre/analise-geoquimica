#include <iostream>
#include <fstream>
#include "input_data.hpp"

int main(int argc, char **argv){
    std::cout << "Analise Geoquimica - Outubro 2021\n";
    auto uso_esperado = "Uso esperado deste programa:\n\tgeoquim.exe caminho_do_arquivo.txt\n";
    std::ifstream input_file;
    if(argc == 2){
        input_file.open(argv[1], std::ifstream::in);
        if(!input_file.is_open()){
            std::cout << "Erro ao abrir o arquivo!\n";
            std::cout << uso_esperado;
            return 1;
        }
    }else{
        std::cout << "Numero de argumentos incorreto!\n";
        std::cout << uso_esperado;
        return 1;
    }
    char input_line[256];
    input_file.getline(input_line, 256);
    input_data::chemical_component a(input_line);
    input_file.close();
    std::cout << a.H;
    return 0;
}