#include <map>
#include "C_ComponenteQuimico.hpp"

#ifndef C_DIST_C_HPP
#define C_DIST_C_HPP

namespace data{
    struct intensity{ // Para ter um valor padrão no mapa
        float x = 0;
    };

    class C_DistC{ // Armazena os dados relevantes de um grupo de componentes com o mesmo C_DBE
        public:
            std::map<int, struct intensity> c_intensity;
            int min_c = 10000; // Guardara o C do componente com o menor C no C_DBE
            int max_c = -1; // Guardara o C do componente com o maior C no C_DBE
            float sum_c(int min, int max, int opt);
            float sum_c(std::vector<int> vals);
            C_DistC();
            void print_intensity_per_c(std::ostream &output);
            float &operator[](int i); 
            void append(C_ComponenteQuimico i);
            // A ser usado na variavel opt da funcao sum_c para escolher entre somar qualquer C, os Impares e os Pares
            static const int all = 0; 
            static const int odd = 1;
            static const int even = 2;
    };
}

#endif