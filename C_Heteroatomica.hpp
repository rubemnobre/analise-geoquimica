#include <map>
#include "C_ComponenteQuimico.hpp"
#include "C_DBE.hpp"

#ifndef C_HETEROATOMICA_HPP
#define C_HETEROATOMICA_HPP

namespace data{
        class C_Heteroatomica{ // Armazena os dados importantes de uma classe heteroatomica
        public:
            component_vector class_components;
            std::string class_name;
            float intensity = 0;
            std::map<int, C_DBE*> class_dbes;
            float sum_dbe(int min, int max);
            float sum_dbe(std::vector<int> indices);
            C_Heteroatomica(component_vector components, std::string name);
            C_DBE *get_DBE(int C_DBE); // Retorna um ponteiro ao C_DBE especificado. Se não existir, inicializa um objeto novo e retorna
            float C(int C_DBE, int c);

            // Chamam a funcao sum_c do C_DBE, simplifica a sintaxe de chamar a funcao do ponteiro dentro do mapa
            float sum_C(int C_DBE, int min, int max, int opt); 
            float sum_C(int C_DBE, std::vector<int> vals);

            void print_intensity_per_dbe(std::ostream &output);
    };
}

#endif