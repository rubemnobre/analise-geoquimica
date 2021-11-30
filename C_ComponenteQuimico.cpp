#include <algorithm>
#include "C_ComponenteQuimico.hpp"

data::C_ComponenteQuimico::C_ComponenteQuimico(std::vector<std::string> input_cols){
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
        }
    }

    DBE = C - H/2.0 + N/2.0 + 1.0;
}

std::string data::C_ComponenteQuimico::to_line(){
    char c_str[5], h_str[5], n_str[5], dbe_str[5];
    std::sprintf(c_str, "%2d", C);
    std::sprintf(h_str, "%2d", H);
    std::sprintf(n_str, "%2d", N);
    std::sprintf(dbe_str, "%2d", DBE);
    return cls + "\t\t" + mol_formula + "  \t" + c_str + "\t" + h_str + "\t" + n_str + "\t" + dbe_str + "\t" + std::to_string(intensity) + "\n";
}


std::vector<std::string> data::get_classes(data::component_vector components){
    auto compare_by_cls = [](data::C_ComponenteQuimico a, data::C_ComponenteQuimico b) {return a.cls < b.cls;};
    std::sort(components.begin(), components.end(), compare_by_cls);

    std::vector<std::string> classes;
    int n = components.size();
    classes.push_back(components[0].cls);
    for(int i = 1; i < n; i++){
        if(components[i].cls != classes.back()) classes.push_back(components[i].cls);
    }
    return classes;
}

float data::total_class_intensity(data::component_vector components, std::string cls){
    float total = 0.0;
    int n = components.size();
    for(int i = 0; i < n; i++){
        if(components[i].cls == cls) total += components[i].intensity;
    }
    return total;
}

data::component_vector data::components_per_class(data::component_vector components, std::string cls){
    data::component_vector output;
    for(auto i : components) if(i.cls == cls) output.push_back(i);
    // auto compare_by_dbe = [](data::C_ComponenteQuimico a, data::C_ComponenteQuimico b) {return a.DBE < b.DBE;};
    // std::sort(output.begin(), output.end(), compare_by_dbe);
    return output;
}