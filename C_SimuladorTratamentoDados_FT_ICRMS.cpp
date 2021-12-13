#include <iostream>
#include <iomanip>
#include <filesystem>
#include <sstream>
#include "C_Amostra.hpp"
#include "C_ComponenteQuimico.hpp"
#include "C_SimuladorTratamentoDados_FT_ICRMS.hpp"
#include "C_EntradaDeDados.hpp"

void C_SimuladorTratamentoDados_FT_ICRMS::relative_abundancy(data::C_Amostra input, data::C_SaidaDeDados out){
    auto file = out.new_output("abundancia_relativa");
    input.print_relative_abundancy(std::cout);  
    input.print_relative_abundancy(file);
    auto graf = out.new_plot("Abundancia Relativa");
    input.print_relative_abundancy(graf.data);
    graf.xlabel("Classe Heteroatômica");
    graf.ylabel("Abundância Relativa (%)");
    graf.histogram();
}

void C_SimuladorTratamentoDados_FT_ICRMS::petrochemical_study(data::C_Amostra input, data::C_SaidaDeDados out){
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

void C_SimuladorTratamentoDados_FT_ICRMS::geochemical_study(data::C_Amostra input, data::C_SaidaDeDados out){
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

C_SimuladorTratamentoDados_FT_ICRMS::C_SimuladorTratamentoDados_FT_ICRMS(){
    data::C_EntradaDeDados data_input;

    std::vector<std::string> possible_files;
    for(auto i : std::filesystem::directory_iterator("./input")){
        possible_files.push_back(i.path().string().substr(8));
    }

    std::string fname;
    while(!data_input.is_successful()){
        std::cout << data_input.error;
        for(int i = 0; i < possible_files.size(); i++){
            std::cout << i << ". " << possible_files[i] << "\n";
        }
        int num = -1;
        while(num < -1 || num >= possible_files.size()){
            std::cout << "Digite o numero do arquivo: ";
            std::cin >> num;
        }
        fname = possible_files[num];
        data_input.open("./input/" + possible_files[num]);
    }
    std::string sname = fname.substr(0, fname.size() - 4);
    input = data_input.get_sample();
    std::cout << "Leitura finalizada!\nDigite o nome do estudo a ser realizado (default: " << sname << "): ";
    std::string study_name;
    std::cin.clear();
    fflush(stdin);
    std::getline(std::cin, study_name);
    if(study_name.size() == 0){
        study_name = sname;
    }
    std::cout << study_name << "\n";
    out = new data::C_SaidaDeDados(study_name, "./output");

    auto file = out->new_output("dados-tratados");
    file << "Dados tratados: \n";
    input->print_modified(file);
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

void C_SimuladorTratamentoDados_FT_ICRMS::all_results(data::C_Amostra linput, std::ostream &file){
    linput.print_relative_abundancy(file);

    int n = linput.class_names.size();
    for(int i = 0; i < n; i++){
        auto hclass = linput.get_class(linput.class_names[i]);
        hclass->print_intensity_per_dbe(file);
    }

    for(int i = 0; i < n; i++){
        auto hclass = linput.get_class(linput.class_names[i]);
        hclass->print_intensity_per_dbe(file);

        for(auto i : hclass->class_dbes){
            auto dbe = hclass->get_DBE(i.first);
            dbe->print_intensity_per_c(file);
        }
    }

    linput.print_biodegradation(file);
    linput.print_paleoenvironment(file);
    linput.print_maturity(file);
}

void C_SimuladorTratamentoDados_FT_ICRMS::generate_test_data(std::string test_folder){
    std::vector<std::string> possible_files;
    for(auto i : std::filesystem::directory_iterator("./input")){
        possible_files.push_back(i.path().string().substr(8));
    }
    for(auto fname : possible_files){
        data::C_EntradaDeDados data_input;
        data_input.open(test_folder + "/" + fname);
        auto linput = *(data_input.get_sample());
        std::string study_name = fname.substr(0, fname.size() - 4);
        auto lout = data::C_SaidaDeDados(study_name, "./teste/results");
        auto file = lout.new_output("all");
        all_results(linput, file);
    }
}

void C_SimuladorTratamentoDados_FT_ICRMS::test(std::string test_folder){
    std::vector<std::string> possible_files;
    for(const auto i : std::filesystem::directory_iterator(test_folder)){
        possible_files.push_back(i.path().string().substr(8));
    }

    int e = 0, n = 0;
    for(int i = 0; i < possible_files.size(); i++){
        data::C_EntradaDeDados data_input;
        data_input.open(test_folder + "/" + possible_files[i]);
        if(data_input.is_successful()){
            auto linput = data_input.get_sample();
            std::string study_name = possible_files[i].substr(0, possible_files[i].size() - 4);
            std::stringstream res, expected;
            all_results(*linput, res);
            auto file = std::ifstream("./teste/results/" + study_name + "_all.txt");
            expected << file.rdbuf();
            file.close();
            n++;
            if(expected.str().compare(res.str()) != 0){
                std::cout << expected.str().compare(res.str()) << "\n";
                e++;
            }
        }
    }
    std::cout << e << " erros em " << n << " testes!\n";
}