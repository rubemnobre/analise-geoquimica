#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <vector>
#include <array>
#include <iomanip>
#include <algorithm>
#include "input_data.hpp"

void relative_abundancy(data::sample input, std::ofstream &file){
    input.print_relative_abundancy(std::cout);
    input.print_relative_abundancy(file);
}

void petrochemical_study(data::sample input, std::ofstream &file){
    int choice = 0;
    std::cout << "1. Calcular abundancia relativa do DBE por classe\n2. Calcular distribuicao de numeros carbono por DBE";
    std::cout << "\nEscolha: ";
    std::cin >> choice;
    if(choice == 1){
        int n = input.class_names.size();
        std::cout << '\n';
        for(int i = 0; i < n; i++){
            std::cout << i << ". " << input.class_names[i] << '\n';
        }
        int i_sel = -1;
        while(i_sel > n - 1 || i_sel < 0){
            std::cout << "Selecione a classe heteroatomica: ";
            std::cin >> i_sel;
        }
        
        auto hclass = *input.classes[input.class_names[i_sel]];
        
        hclass.print_intensity_per_dbe(std::cout);
        hclass.print_intensity_per_dbe(file);
    }
    if(choice == 2){
        int n = input.class_names.size();
        std::cout << '\n';
        for(int i = 0; i < n; i++){
            std::cout << i << ". " << input.class_names[i] << '\n';
        }
        int i_sel = -1;
        while(i_sel > n - 1 || i_sel < 0){
            std::cout << "Selecione a classe heteroatomica: ";
            std::cin >> i_sel;
        }
        
        auto hclass = *input.classes[input.class_names[i_sel]];

        int dbe_sel = 0;
        std::cout << "\nSelecione o DBE para analisar a distribuicao por numero de carbono: ";
        std::cin >> dbe_sel;

        auto dbe = *hclass.class_dbes[dbe_sel];

        dbe.print_intensity_per_c(std::cout);
        dbe.print_intensity_per_c(file);
    }
}

void geochemical_study(data::sample input, std::ofstream &file){
    std::cout << std::fixed << std::setprecision(6);
    char opt = '0';
    while(opt != '4'){
        std::cout << "1. Calcular a Biodegradacao\n" <<
                     "2. Calcular a Paleoambiente\n" <<
                     "3. Calcular a Maturidade\n" <<
                     "4. Sair\nDigite a escolha: ";
        std::cin >> opt;
        if(opt == '1'){
            input.print_biodegradation(std::cout);
            input.print_biodegradation(file);
        }
        if(opt == '2'){
            input.print_paleoenvironment(std::cout);
            input.print_paleoenvironment(file);
        }
        if(opt == '3'){
            input.print_maturity(std::cout);
            input.print_maturity(file);
        }
    }
}

int main(int argc, char **argv){
    std::cout << "Analise Geoquimica - Outubro 2021\n";

    std::ifstream input_file;
    std::string nome;
    while(!input_file.is_open()){
        std::cout << "Digite o nome do arquivo: ";
        std::cin >> nome;
        input_file.open("./input/" + nome, std::ifstream::in);
    }

    std::cout << "Lendo os dados do arquivo " << nome << "!\n";
    auto components = data::read_ifstream(&input_file);
    auto input = data::sample(components);
    input_file.close();
    std::cout << "Leitura finalizada!\nDigite o nome do arquivo para salvar os dados tratados: ";
    std::ofstream new_file;
    do{
        std::string new_path;
        std::cin >> new_path;
        new_file.open("./output/" + new_path, std::ofstream::out);
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
                relative_abundancy(input, new_file);
                break;
            case '2':
                petrochemical_study(input, new_file);
                break;
            case '3':
                geochemical_study(input, new_file);
                break;
        }
    }

    new_file << "Dados tratados: \n";
    data::write_modified(components, &new_file);
    std::cout << "Dados tratados salvos!\n";
    new_file.close();
    return 0;
}