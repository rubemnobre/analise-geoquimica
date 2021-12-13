#include <iostream>
#include <iomanip>
#include "C_Amostra.hpp"
#include "C_ComponenteQuimico.hpp"
#include "C_SimuladorTratamentoDados_FT_ICRMS.hpp"

void C_SimuladorTratamentoDados_FT_ICRMS::relative_abundancy(data::C_Amostra input, data::C_SaidaDeDados out){
    auto file = out.new_output("abundancia-relativa");
    input.print_relative_abundancy(std::cout);
    input.print_relative_abundancy(file);
}

void C_SimuladorTratamentoDados_FT_ICRMS::petrochemical_study(data::C_Amostra input, data::C_SaidaDeDados out){
    auto file = out.new_output("estudo-petroquimico");
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

            int dbe_sel = 0;
            std::cout << "\nSelecione o DBE para analisar a distribuicao por numero de carbono: ";
            std::cin >> dbe_sel;

            auto dbe = *hclass.get_DBE(dbe_sel);

            dbe.print_intensity_per_c(std::cout);
            dbe.print_intensity_per_c(file);
        }
    }
}

void C_SimuladorTratamentoDados_FT_ICRMS::geochemical_study(data::C_Amostra input, data::C_SaidaDeDados out){
    auto file = out.new_output("abundancia-relativa");
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

C_SimuladorTratamentoDados_FT_ICRMS::C_SimuladorTratamentoDados_FT_ICRMS(){
    std::ifstream input_file;
    std::string nome;
    while(!input_file.is_open()){
        std::cout << "Digite o nome do arquivo: ";
        std::cin >> nome;
        input_file.open("./input/" + nome, std::ifstream::in);
    }

    std::cout << "Lendo os dados do arquivo " << nome << "!\n";
    components = data::read_ifstream(&input_file);
    input = new data::C_Amostra(components);
    input_file.close();
    std::cout << "Leitura finalizada!\nDigite o nome do estudo a ser realizado: ";
    std::string study_name;
    std::cin >> study_name;
    out = new data::C_SaidaDeDados(study_name, "./output");

    auto file = out->new_output("dados-tratados");
    file << "Dados tratados: \n";
    data::write_modified(components, &file);
    std::cout << "Dados tratados salvos!\n";
    file.close();
}

void C_SimuladorTratamentoDados_FT_ICRMS::main_loop(){
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
                relative_abundancy(*input, *out);
                break;
            case '2':
                petrochemical_study(*input, *out);
                break;
            case '3':
                geochemical_study(*input, *out);
                break;
        }
    }
}