#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <vector>
#include <array>
#include <iomanip>
#include "input_data.hpp"

void relative_abundancy(std::vector<data::chemical_component> components, std::ofstream *file){
    auto classes = analysis::get_classes(components);
    const int n = classes.size();
    std::vector<float> abundancies;
    float total = 0.0;
    for(int i = 0; i < n; i++){
        abundancies.push_back(analysis::total_class_intensity(components, classes[i]));
        total += abundancies[i];
    }

    *file << std::fixed << std::setprecision(6);
    *file << "\nAbundancia das classes heteroatomicas\n";
    *file << "Intensidade total: " << total << "\n";
    *file << "Classe\tInt. Relativa\n";
    for(int i = 0; i < n; i++){
        *file << classes[i] << '\t' << abundancies[i]/total << "\n";
    } 
    *file << '\n';

    std::cout << std::fixed << std::setprecision(6);
    std::cout << "\nIntensidade total: " << total << "\n";
    std::cout << "Classe\tInt. Relativa\n";
    for(int i = 0; i < n; i++){
        std::cout << classes[i] << '\t' << abundancies[i]/total << "\n";
    } 
    std::cout << '\n';
}

void petrochemical_study(std::vector<data::chemical_component> components, std::ofstream *file){
    
}

void geochemical_study(std::vector<data::chemical_component> components, std::ofstream *file){
    
}

int main(int argc, char **argv){
    std::cout << "Analise Geoquimica - Outubro 2021\n";

    std::ifstream input_file;
    std::string nome;
    while(!input_file.is_open()){
        std::cout << "Digite o nome do arquivo: ";
        std::cin >> nome;
        input_file.open(nome, std::ifstream::in);
    }

    std::cout << "Lendo os dados do arquivo " << nome << "!\n";
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
                relative_abundancy(components, &new_file);
                break;
            case '2':
                petrochemical_study(components, &new_file);
                break;
            case '3':
                geochemical_study(components, &new_file);
                break;
        }
    }
    new_file.close();
    return 0;
}