#include <algorithm>
#include <stdexcept>
#include <ostream>
#include "C_DBE.hpp"
#include "C_Amostra.hpp"

data::C_DBE::C_DBE(data::component_vector components, int dbe){
    dbe_components = components;
    if(components.size() > 0) parent_class = components[0].cls;
    val = dbe;

    // Ordena os componentes do DBE por número de carbono
    auto compare_by_c = [](data::C_ComponenteQuimico a, data::C_ComponenteQuimico b) {return a.C < b.C;};
    std::sort(components.begin(), components.end(), compare_by_c);
    
    if(components.size() > 0){
        int prev_c = components.front().C;
        c_intensity[prev_c].x = 0.0;
        for(auto i : components){
            if(i.DBE == val){
                if(i.C != prev_c){
                    c_intensity[i.C].x = 0.0;
                }
                intensity += i.intensity;
                c_intensity[i.C].x += i.intensity;
                if(i.C > max_c) max_c = i.C;
                if(i.C < min_c) min_c = i.C;
            }
        }
    }
}

float data::C_DBE::sum_c(int min, int max, int opt){
    float sum = 0.0;
    if(opt == all) for(int i = min; i <= max; i++) sum += c_intensity[i].x;

    if(opt == even) for(int i = min%2?min+1:min; i <= max; i += 2) sum += c_intensity[i].x;

    if(opt == odd) for(int i = min%2?min:min+1; i <= max; i += 2) sum += c_intensity[i].x;

    return sum;
}

float data::C_DBE::sum_c(std::vector<int> vals){
    float sum = 0.0;
    for(auto i : vals) sum += c_intensity[i].x;
    return sum;
}

data::C_DBE *data::C_Amostra::get_DBE(int num){
    try {
        return dbes.at(num);
    }
    catch (const std::out_of_range& oor) {
        return new data::C_DBE(C_Amostra_components, num);
    }
}

data::C_DBE *data::C_Heteroatomica::get_DBE(int num){
    try {
        return class_dbes.at(num);
    }
    catch (const std::out_of_range& oor) {
        return new data::C_DBE(class_components, num);
    }
}

void data::C_DBE::print_intensity_per_c(std::ostream &output){
    output << "Distribuicao da quantidade de carbonos no DBE" << val << " da classe " << parent_class << '\n';
    output << "Num. C\tIntensidade\n";
    for(int i = min_c; i <= max_c; i++){
        output << i << '\t' << c_intensity[i].x << '\n';
    }
    output << '\n';
}