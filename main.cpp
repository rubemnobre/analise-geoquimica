#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
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

    std::cout << "Lendo os dados do arquivo " << argv[1] << "!\n";
    auto components = data::read_ifstream(&input_file);
    input_file.close();
    std::cout << "Leitura finalizada!\nDigite o nome do arquivo para salvar os dados tratados: ";
    std::ofstream new_file;
    do{
        char new_path[50];
        std::cin >> new_path;
        new_file.open(new_path, std::ofstream::out);
    }while(!new_file.is_open());

    data::write_modified(components, &new_file);
    new_file.close();
    
    std::cout << "Dados tratados salvos!\n";

    return 0;
}