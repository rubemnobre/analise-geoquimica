#include <map>
#include "C_ComponenteQuimico.hpp"

#ifndef C_DBE_HPP
#define C_DBE_HPP

namespace data{
    struct intensity{ // Para ter um valor padrão no mapa
        float x = 0;
    };

    class C_DBE{ // Armazena os dados relevantes de um grupo de componentes com o mesmo C_DBE
        public:
            component_vector dbe_components;
            std::string parent_class = "";
            int val;
            float intensity = 0;
            std::map<int, struct intensity> c_intensity;
            int min_c = 10000; // Guardara o C do componente com o menor C no C_DBE
            int max_c = -1; // Guardara o C do componente com o maior C no C_DBE
            float sum_c(int min, int max, int opt);
            float sum_c(std::vector<int> vals);
            C_DBE(component_vector components, int C_DBE);
            void print_intensity_per_c(std::ostream &output);

            // A ser usado na variavel opt da funcao sum_c para escolher entre somar qualquer C, os Impares e os Pares
            static const int all = 0; 
            static const int odd = 1;
            static const int even = 2;
    };
}

#endif