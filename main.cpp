#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <vector>
#include <array>
#include <iomanip>
#include <algorithm>
#include "C_Amostra.hpp"
#include "C_EntradaDeDados.hpp"
#include "C_SaidaDeDados.hpp"
#include "C_Grafico.hpp"

void relative_abundancy(data::C_Amostra input, data::C_SaidaDeDados out){
    auto file = out.new_output("abundancia_relativa");
    input.print_relative_abundancy(std::cout);  
    input.print_relative_abundancy(file);
    auto graf = out.new_plot("Abundancia Relativa");
    input.print_relative_abundancy(graf.data);
    graf.xlabel("Classe Heteroatômica");
    graf.ylabel("Abundância Relativa (%)");
    graf.histogram();
}

void petrochemical_study(data::C_Amostra input, data::C_SaidaDeDados out){
    auto file = out.new_output("estudo_petroquimico");
    int choice = 0;
    while(choice != 3){
        std::cout << "1. Calcular abundancia relativa do DBE por classe\n2. Calcular distribuicao de numeros carbono por DBE\n3.Voltar";
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
            
            auto hclass = *input.get_class(input.class_names[i_sel]);
            
            hclass.print_intensity_per_dbe(std::cout);
            hclass.print_intensity_per_dbe(file);
            auto graf = out.new_plot(hclass.class_name + " Intensidade por DBE");
            hclass.print_intensity_per_dbe(graf.data);
            graf.xlabel("DBE");
            graf.ylabel("Abundância Relativa (%)");
            graf.histogram();
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
            
            auto hclass = *input.get_class(input.class_names[i_sel]);

            std::cout << "DBEs da classe:\n";
            for(auto i : hclass.class_dbes){
                std::cout << i.first << '\n';
            }

            int dbe_sel = 0;
            std::cout << "\nSelecione o DBE para analisar a distribuicao por numero de carbono: ";
            std::cin >> dbe_sel;

            auto dbe = *hclass.get_DBE(dbe_sel);

            dbe.print_intensity_per_c(std::cout);
            dbe.print_intensity_per_c(file);
            auto graf = out.new_plot("Classe " + hclass.class_name + " DBE " + std::to_string(dbe.val) + " intensidade por C");
            dbe.print_intensity_per_c(graf.data);
            graf.xlabel("Número de Carbonos");
            graf.ylabel("Intensidade");
            graf.histogram();
        }
    }
}

void geochemical_study(data::C_Amostra input, data::C_SaidaDeDados out){
    auto file = out.new_output("estudo_geoquimico");
    std::cout << std::fixed << std::setprecision(6);
    char opt = '0';
    while(opt != '4'){
        std::cout << "1. Calcular a Biodegradacao\n" <<
                     "2. Calcular a Paleoambiente\n" <<
                     "3. Calcular a Maturidade\n" <<
                     "4. Voltar\nDigite a escolha: ";
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

    data::C_EntradaDeDados data_input;
    while(!data_input.is_successful()){
        std::cout << data_input.error;
        std::string fname;
        std::cout << "Digite o nome do arquivo: ";
        std::cin >> fname;
        data_input.open("./input/" + fname);
    }

    auto input = data_input.get_sample();
    std::cout << "Leitura finalizada!\nDigite o nome do estudo a ser realizado: ";
    std::string study_name;
    std::cin >> study_name;
    auto out = data::C_SaidaDeDados(study_name, "./output");
    
    auto file = out.new_output("dados-tratados");
    file << "Dados tratados: \n";
    input.print_modified(file);
    std::cout << "Dados tratados salvos!\n";
    file.close();

    std::cout << std::fixed << std::setprecision(6);

    char opt = 0;
    while(opt != '4'){
        std::cout << "1. Calcular a abundancia relativa das Classes Heteroatomicas\n" <<
                    "2. Estudo Petroleoquimico\n" <<
                    "3. Estudo Geoquimico\n" <<
                    "4. Sair\nEscolha a atividade a ser realizada: ";
        std::cin >> opt;

        switch(opt){
            case '1':
                relative_abundancy(input, out);
                break;
            case '2':
                petrochemical_study(input, out);
                break;
            case '3':
                geochemical_study(input, out);
                break;
        }
    }

    return 0;
}