#include "input_data.hpp"
#include <iostream>
#include <fstream>
#include <array>
#include <vector>
#include <string>
#include <algorithm>
#include <cstdio>
#include <cctype>

#define MAX_WIDTH 100

std::vector<std::string> split_line(char *str){
    std::vector<std::string> output;
    int last = 0;
    int i_col = 0;
    for(int i = 0; str[i] != '\0' && i_col < N_COLS - 1; i++){
        if(isspace(str[i])){
            do{
                i++;
            }while(isspace(str[i]));
            output.push_back(std::string(&str[last], (i - 1) - last));
            i_col++;
            last = i;
        }
    }
    output.push_back(std::string(&str[last]));
    return output;
}

chemical_component::chemical_component(std::vector<std::string> input_cols){
    cls = input_cols[0]; // Classe
    mol_formula = input_cols[5]; // Formula molecular
    std::replace(input_cols[6].begin(), input_cols[6].end(), ',', '.'); // Substituir ponto e virgula para não assassinar o std::stof()
    intensity = std::stof(input_cols[6]);

    // Separa os numeros em substrings para rodar a funcao std::stod()
    size_t c_ind = mol_formula.find('C');
    size_t h_ind = mol_formula.find('H');
    size_t n_ind = mol_formula.find('N');
    size_t o_ind = mol_formula.find('O');
    size_t s_ind = mol_formula.find('S');

    C = std::stod(mol_formula.substr(c_ind + 1, h_ind - (c_ind + 1)));

    size_t after_h = std::min({n_ind, o_ind, s_ind, mol_formula.size()-1}); // Procura o proximo indice depois de h
    
    H = std::stod(mol_formula.substr(h_ind + 1, after_h - (h_ind + 1)));
    
    if(n_ind == std::string::npos) N = 0;
    else{
        int after_n = std::min({o_ind, s_ind, mol_formula.size()-1}); // Procura o proximo indice depois de n
        if(after_n - n_ind < 2){
            N = 1;
        }else{
            N = std::stod(mol_formula.substr(n_ind + 1, after_n - (n_ind + 1)));
            int a;
            std::cin >> a;
        }
    }

    DBE = C - H/2.0 + N/2.0 + 1.0;
}

component_vector read_ifstream(std::ifstream *input){
    component_vector data_vector;
    char header[MAX_WIDTH];
    input->getline(header, MAX_WIDTH);
    while(!input->eof()){
        char input_line[MAX_WIDTH];
        input->getline(input_line, MAX_WIDTH);
        auto input_cols = split_line(input_line);
        if(input_cols.size() == 7){
            data_vector.push_back(chemical_component(input_cols));
        }
    }
    return data_vector;
}

std::string chemical_component::to_line(){
    char c_str[5], h_str[5], n_str[5], dbe_str[5];
    std::sprintf(c_str, "%2d", C);
    std::sprintf(h_str, "%2d", H);
    std::sprintf(n_str, "%2d", N);
    std::sprintf(dbe_str, "%2d", DBE);
    return cls + "\t\t" + mol_formula + "  \t" + c_str + "\t" + h_str + "\t" + n_str + "\t" + dbe_str + "\t" + std::to_string(intensity) + "\n";
}

void write_modified(component_vector components, std::ofstream *file){
    int n = components.size();
    *file << "Class\tMol. Formula\tC\tH\tN\tDBE\tIntensity\n";
    for(int i = 0; i < n; i++){
        *file << components[i].to_line();
    }
}
