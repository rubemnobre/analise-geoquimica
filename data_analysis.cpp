#include "input_data.hpp"
#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <iomanip>

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
    if(components.size() > 0) parent_class = components[0].cls;
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

void data::sample::print_relative_abundancy(std::ostream &output){
    output << std::fixed << std::setprecision(6);
    output << "\nAbundancia das classes heteroatomicas\n";
    output << "Intensidade total: " << intensity << "\n";
    output << "Classe\tInt. Relativa\n";
    for(auto i : classes){
        output << i.first << '\t' << i.second->intensity/intensity << "\n";
    }

    output << '\n';
}

void data::sample::print_biodegradation(std::ostream &output){
    auto O2 = *get_class("O2");
    auto O1 = *get_class("O1");
    output << "Indices de biodegradacao:\n";
    output << "A/C: " << (O2.get_DBE(1)->intensity)/O2.sum_dbe(2, 4) << "\n";
    output << "A/C mod.: " << (O2.get_DBE(1)->intensity)/O2.sum_dbe(2, 6) << "\n";
    output << "Indice S/A: " << O2.sum_dbe(1, 6) << "\n";
    output << "S/A mod.: " << O2.sum_dbe(2, 6) << "\n";
    output << "Indice MA1: " << (O1.get_DBE(4)->intensity)/(O1.get_DBE(5)->intensity) << "\n";
    output << "Indice MA2: " << (O1.get_DBE(4)->intensity)/(O1.get_DBE(7)->intensity) << "\n";
}

void data::sample::print_paleoenvironment(std::ostream &output){
    auto O2 = *get_class("O2");
    auto O1 = *get_class("O1");
    auto NO = *get_class("NO");
    auto N = *get_class("N");

    output << "Indices de paleoambiente:\n";
    output << "Indice Phenol: " << (O1.get_DBE(4)->intensity)/O1.intensity << "\n";
    output << "C27/C28 (DBE4): " << (O1.C(4, 27))/(O1.C(4, 28)) << "\n";
    output << "C27/C28 (DBE5): " << (O1.C(5, 27))/(O1.C(5, 28)) << "\n";
    output << "Par/Impar (FA): " << (O2.sum_C(1, 20, 36, data::DBE::even))/(O2.sum_C(1, 19, 35, data::DBE::odd)) << "\n";
    output << "OEP (FA): " << (O2.C(1, 22) + 6.0*O2.C(1, 28) + O2.C(1, 26))/(4*O2.C(1, 23) + 4*O2.C(1, 25)) << "\n";
    output << "TAR (FA) Par: " << (O2.sum_C(1, {24, 26, 28}))/(O2.sum_C(1, {12, 14, 16})) << "\n";
    output << "TAR (FA) Impar: " << (O2.sum_C(1, {27, 29, 31}))/(O2.sum_C(1, {15, 17, 19})) << "\n";
    output << "C36 Indice de Acido Hepanoico: " << O2.C(6, 36)/O2.C(6, 31) << "\n";
    output << "Hopano / Acido Estanoico: " << O2.C(6, 31)/O2.C(5, 28) << "\n";
    output << "Razao Rocha 1: " << O2.sum_dbe(1, 7)/(O2.sum_dbe(1, 7) + O2.sum_dbe(8, 25)) << "\n";
    output << "Razao Rocha 2: " << O1.sum_dbe(4, 10)/(O1.sum_dbe(4, 10) + O1.sum_dbe(11, 23)) << "\n";
    output << "Razao Rocha 3: " << NO.get_DBE(10)->intensity << "\n";
    output << "Razao Rocha 4: " << O1.get_DBE(4)->intensity << "\n";
    output << "Razao Rocha 5: " << N.sum_dbe(6, 10)/(N.sum_dbe(6, 10) + N.sum_dbe(10, 27)) << "\n";
    output << "Razao Rocha 6: " << N.sum_dbe(2, 14)/(N.sum_dbe(2, 14) + N.sum_dbe(15, 29)) << "\n";
    output << "Razao Rocha 7: " << O1.sum_dbe(4, 12) + O2.get_DBE(12)->intensity/(O1.sum_dbe({4, 12, 14, 16}) + O2.sum_dbe({12, 13})) << "\n";
    output << "Razao Rocha 8: " << N.sum_dbe({8, 9}) + NO.sum_dbe({9, 10})/(N.sum_dbe({8, 9, 13, 16}) + NO.sum_dbe({9, 10, 19, 20})) << "\n";
}

void data::sample::print_maturity(std::ostream &output){
    auto N = *get_class("N");

    output << "Indices de maturidade:\n";
    output << "DBE9  da Classe N: " << N.get_DBE(9)->intensity << "\n";
    output << "DBE12 da Classe N: " << N.get_DBE(12)->intensity << "\n";
    output << "DBE15 da Classe N: " << N.get_DBE(15)->intensity << "\n";
}

void data::heteroatomic_class::print_intensity_per_dbe(std::ostream &output){
    output << "\nAbundancia relativa do DBE na Classe Heteroatomica " << class_name;
    output << "\nIntensidade total da classe: " << intensity << "\nDBE\tInt. Relativa\n";
    for(auto i : class_dbes){
        output     << i.first << '\t' << i.second->intensity/intensity << '\n';
    }
    output << '\n';
}

void data::DBE::print_intensity_per_c(std::ostream &output){
    output << "Distribuicao da quantidade de carbonos no DBE" << val << " da classe" << parent_class << '\n';
    output << "Num. C\tIntensidade\n";
    for(int i = min_c; i <= max_c; i++){
        output << i << '\t' << c_intensity[i].x << '\n';
    }
    output << '\n';
}