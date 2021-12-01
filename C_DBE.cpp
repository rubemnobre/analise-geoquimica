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
        c_distribution[prev_c] = 0.0;
        for(auto i : components){
            if(i.DBE == val){
                intensity += i.intensity;
                c_distribution.append(i);
            }
        }
    }
}

float data::C_DBE::sum_c(int min, int max, int opt){
    return c_distribution.sum_c(min, max, opt);
}

float data::C_DBE::sum_c(std::vector<int> vals){
    float sum = 0.0;
    for(auto i : vals) sum += c_distribution[i];
    return sum;
}

data::C_DBE *data::C_Amostra::get_DBE(int num){
    try {
        return dbes.at(num);
    }
    catch (const std::out_of_range& oor) {
        return new data::C_DBE(sample_components, num);
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
    for(int i = c_distribution.min_c; i <= c_distribution.max_c; i++){
        output << i << '\t' << c_distribution[i] << '\n';
    }
    output << '\n';
}
