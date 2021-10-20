#include "input_data.hpp"

std::vector<std::string> analysis::get_classes(std::vector<data::chemical_component> components){
    std::vector<std::string> classes;
    int n = components.size();
    classes.push_back(components[0].cls);
    for(int i = 1; i < n; i++){
        if(components[i].cls != classes.back()) classes.push_back(components[i].cls);
    }
    return classes;
}

float analysis::total_class_intensity(std::vector<data::chemical_component> components, std::string cls){
    float total = 0.0;
    int n = components.size();
    for(int i = 0; i < n; i++){
        if(components[i].cls == cls) total += components[i].intensity;
    }
    return total;
}