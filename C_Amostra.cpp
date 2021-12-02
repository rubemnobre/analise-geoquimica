#include <algorithm>
#include <fstream>
#include <iomanip>
#include "C_Amostra.hpp"

data::C_Amostra::C_Amostra(data::component_vector components){
    sample_components = components;
    class_names = data::get_classes(components);
    for(auto i : class_names){
        classes[i] = new data::C_Heteroatomica(components, i);
    }
    
    auto compare_by_dbe = [](data::C_ComponenteQuimico a, data::C_ComponenteQuimico b) {return a.DBE < b.DBE;};
    std::sort(components.begin(), components.end(), compare_by_dbe);
    
    intensity = 0;

    if(components.size() > 0){
        int dbe_ant = components[0].DBE;
        dbes[dbe_ant] = new data::C_DBE(components, dbe_ant);
        for(auto i : components){
            if(i.DBE != dbe_ant){
                dbes[i.DBE] = new data::C_DBE(components, i.DBE);
            }
            dbe_ant = i.DBE;
            intensity += i.intensity;
        }
    }
}

void data::C_Amostra::print_relative_abundancy(std::ostream &output){
    output << std::fixed << std::setprecision(6);
    output << "\n#Abundancia das classes heteroatomicas\n";
    output << "#Intensidade total: " << intensity << "\n";
    output << "#Classe\tInt. Relativa\n";
    for(auto i : classes){
        output << i.first << '\t' << i.second->intensity/intensity << "\n";
    }

    output << '\n';
}

void data::C_Amostra::print_biodegradation(std::ostream &output){
    output << std::fixed << std::setprecision(6);
    auto O2 = *get_class("O2");
    auto O1 = *get_class("O");
    output << "Indices de biodegradacao:\n";
    output << "A/C: " << (O2.get_DBE(1)->intensity)/O2.sum_dbe(2, 4) << "\n";
    output << "A/C mod.: " << (O2.get_DBE(1)->intensity)/O2.sum_dbe(2, 6) << "\n";
    output << "Indice S/A: " << O2.sum_dbe(1, 6) << "\n";
    output << "S/A mod.: " << O2.sum_dbe(2, 6) << "\n";
    output << "Indice MA1: " << (O1.get_DBE(4)->intensity)/(O1.get_DBE(5)->intensity) << "\n";
    output << "Indice MA2: " << (O1.get_DBE(4)->intensity)/(O1.get_DBE(7)->intensity) << "\n";
}

void data::C_Amostra::print_paleoenvironment(std::ostream &output){
    auto O2 = *get_class("O2");
    auto O1 = *get_class("O");
    auto NO = *get_class("NO");
    auto N = *get_class("N");

    output << "Indices de paleoambiente:\n";
    output << "Indice Phenol: " << (O1.get_DBE(4)->intensity)/O1.intensity << "\n";
    output << "C27/C28 (DBE4): " << (O1.C(4, 27))/(O1.C(4, 28)) << "\n";
    output << "C27/C28 (DBE5): " << (O1.C(5, 27))/(O1.C(5, 28)) << "\n";
    output << "Par/Impar (FA): " << (O2.sum_C(1, 20, 36, data::C_DBE::even))/(O2.sum_C(1, 19, 35, data::C_DBE::odd)) << "\n";
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
    output << "Razao Rocha 6: " << NO.sum_dbe(2, 14)/(NO.sum_dbe(2, 14) + NO.sum_dbe(15, 29)) << "\n";
    output << "Razao Rocha 7: " << (O1.sum_dbe(4, 12) + O2.get_DBE(12)->intensity)/(O1.sum_dbe({4, 12, 14, 16}) + O2.sum_dbe({12, 13})) << "\n";
    output << "Razao Rocha 8: " << (N.sum_dbe({8, 9}) + NO.sum_dbe({9, 10}))/(N.sum_dbe({8, 9, 13, 16}) + NO.sum_dbe({9, 10, 19, 20})) << "\n";
}

void data::C_Amostra::print_maturity(std::ostream &output){
    auto N = *get_class("N");

    output << "Indices de maturidade:\n";
    output << "DBE9  da Classe N: " << N.get_DBE(9)->intensity << "\n";
    output << "DBE12 da Classe N: " << N.get_DBE(12)->intensity << "\n";
    output << "DBE15 da Classe N: " << N.get_DBE(15)->intensity << "\n";
}

void data::C_Amostra::print_modified(std::ostream &file){
    int n = sample_components.size();
    file << "Class\tMol. Formula\tC\tH\tN\tDBE\tIntensity\n";
    for(int i = 0; i < n; i++){
        file << sample_components[i].to_line();
    }
}