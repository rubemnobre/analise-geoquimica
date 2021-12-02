#include <algorithm>
#include <stdexcept>
#include <ostream>
#include "C_Heteroatomica.hpp"
#include "C_Amostra.hpp"

data::C_Heteroatomica::C_Heteroatomica(data::component_vector components, std::string name){
    for(auto i : components) if(i.cls == name) class_components.push_back(i);
    auto compare_by_dbe = [](data::C_ComponenteQuimico a, data::C_ComponenteQuimico b) {return a.DBE < b.DBE;};
    std::sort(class_components.begin(), class_components.end(), compare_by_dbe);
    class_name = name;
    intensity = 0;
    if(class_components.size() > 0){
        int dbe_ant = class_components.front().DBE;
        class_dbes[dbe_ant] = new data::C_DBE(class_components, dbe_ant);
        for(auto i : class_components){
            if(i.DBE != dbe_ant){
                class_dbes[i.DBE] = new data::C_DBE(class_components, i.DBE);
            }
            intensity += i.intensity;
            dbe_ant = i.DBE;
        }
    }
}

void data::C_Heteroatomica::print_intensity_per_dbe(std::ostream &output){
    output << "\n#Abundancia relativa do DBE na Classe Heteroatomica " << class_name;
    output << "\n#Intensidade total da classe: " << intensity << "\nDBE\tInt. Relativa (%)\n";
    for(auto i : class_dbes){
        output << i.first << '\t' << 100*get_DBE(i.first)->intensity/intensity << '\n';
    }
    output << '\n';
}

float data::C_Heteroatomica::sum_dbe(int min, int max){
    float sum = 0.0;
    for(int i = min; i <= max; i++) sum += get_DBE(i)->intensity;
    return sum;
}

float data::C_Heteroatomica::sum_dbe(std::vector<int> vals){
    float sum = 0.0;
    for(auto i : vals) sum += get_DBE(i)->intensity;
    return sum;
}


data::C_Heteroatomica *data::C_Amostra::get_class(std::string name){
    try {
        return classes.at(name);
    }
    catch (const std::out_of_range& oor) {
        return new data::C_Heteroatomica(sample_components, name);
    }
}


float data::C_Heteroatomica::C(int dbe, int c){
    return get_DBE(dbe)->c_distribution[c];
}

float data::C_Heteroatomica::sum_C(int dbe, int min, int max, int opt){
    return get_DBE(dbe)->sum_c(min, max, opt);
}

float data::C_Heteroatomica::sum_C(int dbe, std::vector<int> vals){
    return get_DBE(dbe)->sum_c(vals);
}