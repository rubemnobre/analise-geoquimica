#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <vector>
#include <array>
#include <iomanip>
#include "input_data.hpp"

void relative_abundancy(std::vector<data::chemical_component> components){
    auto classes = analysis::get_classes(components);
    const int n = classes.size();
    std::vector<float> abundancies;
    float total = 0.0;
    for(int i = 0; i < n; i++){
        abundancies.push_back(analysis::total_class_intensity(components, classes[i]));
        total += abundancies[i];
    }
    
    std::cout << std::fixed << std::setprecision(6);
    std::cout << "\nIntensidade total: " << total << "\n";
    std::cout << "Classe\tInt. Relativa\n";
    for(int i = 0; i < n; i++){
        std::cout << classes[i] << '\t' << abundancies[i]/total << "\n";
    } 
    std::cout << '\n';
}

void petrochemical_study(std::vector<data::chemical_component> components){
    
}

void geochemical_study(std::vector<data::chemical_component> components){
    
}

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
    
    std::cout << "Dados tratados salvos!\n\n";

    char opt = 0;
    while(opt != '4'){
        std::cout << "1. Calcular a abundancia relativa das Classes Heteroatomicas\n" <<
                    "2. Estudo Petroleoquimico\n" <<
                    "3. Estudo Geoquimico\n" <<
                    "4. Sair\nEscolha a atividade a ser realizada: ";
        std::cin >> opt;

        switch(opt){
            case '1':
                relative_abundancy(components);
                break;
            case '2':
                petrochemical_study(components);
                break;
            case '3':
                geochemical_study(components);
                break;
        }
    }

    return 0;
}