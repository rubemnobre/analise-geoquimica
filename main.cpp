#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <vector>
#include <array>
#include <iomanip>
#include <algorithm>
#include "input_data.hpp"

void relative_abundancy(data::component_vector components, std::ofstream *file){
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
    std::cout << "\nIntensidade total: " << total << "\n";
    std::cout << "Classe\tInt. Relativa\n";
    for(int i = 0; i < n; i++){
        *file << classes[i] << '\t' << abundancies[i]/total << "\n";
        std::cout << classes[i] << '\t' << abundancies[i]/total << "\n";
    }

    *file << '\n';
    std::cout << '\n';
}

void petrochemical_study(data::component_vector components, std::ofstream *file){
    int choice = 0;
    std::cout << "1. Calcular abundancia relativa do DBE por classe\n2. Calcular distribuicao de numeros carbono por DBE";
    std::cout << "\nEscolha: ";
    std::cin >> choice;
    if(choice == 1){
        auto classes = analysis::get_classes(components);
        int n = classes.size();
        std::cout << '\n';
        for(int i = 0; i < n; i++){
            std::cout << i << ". " << classes[i] << '\n';
        }
        int i_sel = -1;
        while(i_sel > n - 1 || i_sel < 0){
            std::cout << "Selecione a classe heteroatomica: ";
            std::cin >> i_sel;
        }

        auto total_class = analysis::total_class_intensity(components, classes[i_sel]);
        auto class_components = analysis::components_per_class(components, classes[i_sel]);
        
        int dbe_ant = class_components[0].DBE;
        float total = 0.0;
        std::cout << "\nIntensidade total da classe: " << total_class << "\nDBE\tInt. Relativa\n";
        *file << "\nAbundancia relativa do DBE na Classe Heteroatomica " << classes[i_sel];
        *file << "\nIntensidade total da classe: " << total_class << "\nDBE\tInt. Relativa\n";
        for(auto i : class_components){
            if(dbe_ant != i.DBE){
                std::cout << dbe_ant << '\t' << total/total_class << '\n';
                *file     << dbe_ant << '\t' << total/total_class << '\n';
                total = 0.0;
            }
            total += i.intensity;
            dbe_ant = i.DBE;
        }
        *file << '\n';
    }
    if(choice == 2){
        int dbe_sel = 0;
        std::cout << "\nSelecione o DBE para analisar a distribuicao por numero de carbono: ";
        std::cin >> dbe_sel;

        auto compare_c = [] (data::chemical_component a, data::chemical_component b) { return a.C < b.C; };
        int min_c = std::min_element(components.begin(), components.end(), compare_c)->C;
        
        int max_c = std::max_element(components.begin(), components.end(), compare_c)->C;
        
        std::cout << "\nDistribuicao da quantidade de carbonos no DBE" << dbe_sel << '\n';
        *file << "Distribuicao da quantidade de carbonos no DBE" << dbe_sel << '\n';
        std::cout << "Num. C\tTotal\n";
        *file << "Num. C\tTotal\n";
        for(int i = min_c; i <= max_c; i++){
            float total = 0.0;
            for(auto j : components) if(j.C == i && j.DBE == dbe_sel) total += j.intensity;
            std::cout << i << '\t' << total << '\n';
            *file << i << '\t' << total << '\n';
        }
        *file << '\n';
        std::cout << '\n';
    }
}

void geochemical_study(data::component_vector components, std::ofstream *file){
    
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

    std::cout << std::fixed << std::setprecision(6);
    new_file << std::fixed << std::setprecision(6);

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

    new_file << "Dados tratados: \n";
    data::write_modified(components, &new_file);
    std::cout << "Dados tratados salvos!\n";
    new_file.close();
    return 0;
}