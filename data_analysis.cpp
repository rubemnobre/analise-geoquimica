#include "input_data.hpp"
#include <algorithm>
#include <iostream>
#include <stdexcept>

std::vector<std::string> data::get_classes(data::component_vector components){
    auto compare_by_cls = [](data::chemical_component a, data::chemical_component b) {return a.cls < b.cls;};
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
    auto compare_by_dbe = [](data::chemical_component a, data::chemical_component b) {return a.DBE < b.DBE;};
    std::sort(output.begin(), output.end(), compare_by_dbe);
    return output;
}

data::heteroatomic_class::heteroatomic_class(data::component_vector components, std::string name){
    for(auto i : components) if(i.cls == name) class_components.push_back(i);

    class_name = name;
    intensity = 0;
    if(class_components.size() > 0){
        int dbe_ant = class_components.back().DBE;
        for(auto i : class_components){
            if(i.DBE != dbe_ant) class_dbes[i.DBE] = new data::DBE(class_components, i.DBE);
            intensity += i.intensity;
        }
    }
}

data::DBE::DBE(data::component_vector components, int dbe){
    dbe_components = components;

    val = dbe;

    // Ordena os componentes do DBE por número de carbono
    auto compare_by_c = [](data::chemical_component a, data::chemical_component b) {return a.C < b.C;};
    std::sort(components.begin(), components.end(), compare_by_c);
    
    if(components.size() > 0){
        int prev_c = components.back().C;
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

data::sample::sample(data::component_vector components){
    sample_components = components;
    class_names = data::get_classes(components);
    for(auto i : class_names){
        classes[i] = new data::heteroatomic_class(components, i);
    }
    
    auto compare_by_dbe = [](data::chemical_component a, data::chemical_component b) {return a.DBE < b.DBE;};
    std::sort(components.begin(), components.end(), compare_by_dbe);
    
    intensity = 0;

    if(components.size() > 0){
        int dbe_ant = components[0].DBE;
        dbes[dbe_ant] = new data::DBE(components, dbe_ant);
        for(auto i : components){
            if(i.DBE != dbe_ant){
                dbes[i.DBE] = new data::DBE(components, i.DBE);
            }
            dbe_ant = i.DBE;
            intensity += i.intensity;
        }
    }
}

float data::heteroatomic_class::sum_dbe(int min, int max){
    float sum = 0.0;
    for(int i = min; i <= max; i++) sum += get_DBE(i)->intensity;
    return sum;
}

float data::heteroatomic_class::sum_dbe(std::vector<int> vals){
    float sum = 0.0;
    for(auto i : vals) sum += get_DBE(i)->intensity;
    return sum;
}

float data::DBE::sum_c(int min, int max, int opt){
    float sum = 0.0;
    if(opt == all) for(int i = min; i <= max; i++) sum += c_intensity[i].x;

    if(opt == even) for(int i = min%2?min+1:min; i <= max; i += 2) sum += c_intensity[i].x;

    if(opt == odd) for(int i = min%2?min:min+1; i <= max; i += 2) sum += c_intensity[i].x;

    return sum;
}

float data::DBE::sum_c(std::vector<int> vals){
    float sum = 0.0;
    for(auto i : vals) sum += c_intensity[i].x;
    return sum;
}

data::heteroatomic_class *data::sample::get_class(std::string name){
    try {
        return classes.at(name);
    }
    catch (const std::out_of_range& oor) {
        return new data::heteroatomic_class(sample_components, name);
    }
}

data::DBE *data::sample::get_DBE(int num){
    try {
        return dbes.at(num);
    }
    catch (const std::out_of_range& oor) {
        return new data::DBE(sample_components, num);
    }
}

data::DBE *data::heteroatomic_class::get_DBE(int num){
    try {
        return class_dbes.at(num);
    }
    catch (const std::out_of_range& oor) {
        return new data::DBE(class_components, num);
    }
}

float data::heteroatomic_class::C(int dbe, int c){
    return get_DBE(dbe)->c_intensity[c].x;
}

float data::heteroatomic_class::sum_C(int dbe, int min, int max, int opt){
    return get_DBE(dbe)->sum_c(min, max, opt);
}

float data::heteroatomic_class::sum_C(int dbe, std::vector<int> vals){
    return get_DBE(dbe)->sum_c(vals);
}