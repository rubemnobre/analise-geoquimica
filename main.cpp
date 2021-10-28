#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <vector>
#include <array>
#include <iomanip>
#include <algorithm>
#include "input_data.hpp"

void relative_abundancy(data::sample input, std::ofstream *file){
    *file << std::fixed << std::setprecision(6);
    *file << "\nAbundancia das classes heteroatomicas\n";
    *file << "Intensidade total: " << input.intensity << "\n";
    *file << "Classe\tInt. Relativa\n";
    std::cout << "\nIntensidade total: " << input.intensity << "\n";
    std::cout << "Classe\tInt. Relativa\n";
    for(auto i : input.classes){
        *file << i.first << '\t' << i.second->intensity/input.intensity << "\n";
        std::cout << i.first << '\t' << i.second->intensity/input.intensity << "\n";
    }

    *file << '\n';
    std::cout << '\n';
}

void petrochemical_study(data::sample input, std::ofstream *file){
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
        
        std::cout << "\nIntensidade total da classe: " << hclass.class_name << "\nDBE\tInt. Relativa\n";
        *file << "\nAbundancia relativa do DBE na Classe Heteroatomica " << hclass.class_name;
        *file << "\nIntensidade total da classe: " << hclass.intensity << "\nDBE\tInt. Relativa\n";
        for(auto i : hclass.class_dbes){
            std::cout << i.first << '\t' << i.second->intensity/hclass.intensity << '\n';
            *file     << i.first << '\t' << i.second->intensity/hclass.intensity << '\n';
        }
        *file << '\n';
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

        std::cout << "\nDistribuicao da quantidade de carbonos no DBE" << dbe_sel << " da classe" << hclass.class_name << '\n';
        *file << "Distribuicao da quantidade de carbonos no DBE" << dbe_sel << " da classe" << hclass.class_name << '\n';
        std::cout << "Num. C\tIntensidade\n";
        *file << "Num. C\tIntensidade\n";
        for(int i = dbe.min_c; i <= dbe.max_c; i++){
            std::cout << i << '\t' << dbe.c_intensity[i].x << '\n';
            *file << i << '\t' << dbe.c_intensity[i].x << '\n';
        }
        *file << '\n';
        std::cout << '\n';
    }
}

void geochemical_study(data::sample input, std::ofstream *file){
    std::cout << std::fixed << std::setprecision(6);
    char opt = '0';
    while(opt != '4'){
        std::cout << "1. Calcular a Biodegradacao\n" <<
                     "2. Calcular a Paleoambiente\n" <<
                     "3. Calcular a Maturidade\n" <<
                     "4. Sair\nDigite a escolha: ";
        std::cin >> opt;
        if(opt == '1'){
            auto O2 = *input.get_class("O2");
            auto O1 = *input.get_class("O1");
            std::cout << "A/C: " << (O2.get_DBE(1)->intensity)/O2.sum_dbe(2, 4) << "\n";
            std::cout << "A/C mod.: " << (O2.get_DBE(1)->intensity)/O2.sum_dbe(2, 6) << "\n";
            std::cout << "Indice S/A: " << O2.sum_dbe(1, 6) << "\n";
            std::cout << "S/A mod.: " << O2.sum_dbe(2, 6) << "\n";
            std::cout << "Indice MA1: " << (O1.get_DBE(4)->intensity)/(O1.get_DBE(5)->intensity) << "\n";
            std::cout << "Indice MA2: " << (O1.get_DBE(4)->intensity)/(O1.get_DBE(7)->intensity) << "\n";
        }
        if(opt == '2'){
            auto O2 = *input.get_class("O2");
            auto O1 = *input.get_class("O1");
            auto NO = *input.get_class("NO");
            auto N = *input.get_class("N");

            std::cout << "Indice Phenol: " << (O1.get_DBE(4)->intensity)/O1.intensity << "\n";
            std::cout << "C27/C28 (DBE4): " << (O1.C(4, 27))/(O1.C(4, 28)) << "\n";
            std::cout << "C27/C28 (DBE5): " << (O1.C(5, 27))/(O1.C(5, 28)) << "\n";
            std::cout << "Par/Impar (FA): " << (O2.sum_C(1, 20, 36, data::DBE::even))/(O2.sum_C(1, 19, 35, data::DBE::odd)) << "\n";
            std::cout << "OEP (FA): " << (O2.C(1, 22) + 6.0*O2.C(1, 28) + O2.C(1, 26))/(4*O2.C(1, 23) + 4*O2.C(1, 25)) << "\n";
            std::cout << "TAR (FA) Par: " << (O2.sum_C(1, {24, 26, 28}))/(O2.sum_C(1, {12, 14, 16})) << "\n";
            std::cout << "TAR (FA) Impar: " << (O2.sum_C(1, {27, 29, 31}))/(O2.sum_C(1, {15, 17, 19})) << "\n";
            std::cout << "C36 Indice de Acido Hepanoico: " << O2.C(6, 36)/O2.C(6, 31) << "\n";
            std::cout << "Hopano / Acido Estanoico: " << O2.C(6, 31)/O2.C(5, 28) << "\n";
            std::cout << "Razao Rocha 1: " << O2.sum_dbe(1, 7)/(O2.sum_dbe(1, 7) + O2.sum_dbe(8, 25)) << "\n";
            std::cout << "Razao Rocha 2: " << O1.sum_dbe(4, 10)/(O1.sum_dbe(4, 10) + O1.sum_dbe(11, 23)) << "\n";
            std::cout << "Razao Rocha 3: " << NO.get_DBE(10)->intensity << "\n";
            std::cout << "Razao Rocha 4: " << O1.get_DBE(4)->intensity << "\n";
            std::cout << "Razao Rocha 5: " << N.sum_dbe(6, 10)/(N.sum_dbe(6, 10) + N.sum_dbe(10, 27)) << "\n";
            std::cout << "Razao Rocha 6: " << N.sum_dbe(2, 14)/(N.sum_dbe(2, 14) + N.sum_dbe(15, 29)) << "\n";
            std::cout << "Razao Rocha 7: " << O1.sum_dbe(4, 12) + O2.get_DBE(12)->intensity/(O1.sum_dbe({4, 12, 14, 16}) + O2.sum_dbe({12, 13})) << "\n";
            std::cout << "Razao Rocha 8: " << N.sum_dbe({8, 9}) + NO.sum_dbe({9, 10})/(N.sum_dbe({8, 9, 13, 16}) + NO.sum_dbe({9, 10, 19, 20})) << "\n";
        }
        if(opt == '3'){
            auto N = *input.get_class("N");
            std::cout << "DBE9  da Classe N: " << N.get_DBE(9)->intensity << "\n";
            std::cout << "DBE12 da Classe N: " << N.get_DBE(12)->intensity << "\n";
            std::cout << "DBE15 da Classe N: " << N.get_DBE(15)->intensity << "\n";
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
                relative_abundancy(input, &new_file);
                break;
            case '2':
                petrochemical_study(input, &new_file);
                break;
            case '3':
                geochemical_study(input, &new_file);
                break;
        }
    }

    new_file << "Dados tratados: \n";
    data::write_modified(components, &new_file);
    std::cout << "Dados tratados salvos!\n";
    new_file.close();
    return 0;
}