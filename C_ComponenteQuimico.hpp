#include <string>
#include <vector>

#ifndef C_COMPONENTE_QUIMICO_HPP
#define C_COMPONENTE_QUIMICO_HPP

namespace data{
    class C_ComponenteQuimico{ // Armazena os dados relevantes de cada componente
        public:
            std::string cls;
            std::string mol_formula;
            float intensity;
            int C;
            int H;
            int N;
            int DBE;
            C_ComponenteQuimico(std::vector<std::string> input_cols); // Obtem os dados de um vetor com as strings de cada coluna de cada linha da entrada
            std::string to_line(); // Imprime os dados em uma string para a saida
    };
    
    typedef std::vector<C_ComponenteQuimico> component_vector;

    std::vector<std::string> get_classes(component_vector components);

    component_vector components_per_class(component_vector components, std::string cls); // Retorna os componentes da classe especificada por cls
    
    float total_class_intensity(component_vector components, std::string cls);
}

#endif