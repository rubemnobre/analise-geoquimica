#include <map>
#include <string>
#include "C_ComponenteQuimico.hpp"
#include "C_DBE.hpp"
#include "C_Heteroatomica.hpp"

#ifndef C_AMOSTRA_HPP
#define C_AMOSTRA_HPP

namespace data{
        class C_Amostra{ // Armazena os dados refinados de uma amostra completa
            public:
                float intensity;
                std::string name;
                component_vector sample_components;
                std::map<std::string, C_Heteroatomica*> classes;
                std::map<int, C_DBE*> dbes;
                std::vector<std::string> class_names;
                C_Amostra(component_vector components);
                C_Heteroatomica *get_class(std::string name); // Se não existir, inicializa um objeto novo e retorna
                C_DBE *get_DBE(int C_DBE); // Se nao existir, inicializa um objeto novo e retorna
                void print_relative_abundancy(std::ostream &output);
                void print_biodegradation(std::ostream &output);
                void print_paleoenvironment(std::ostream &output);
                void print_maturity(std::ostream &output);
                void print_modified(std::ostream &output);
    };  
}

#endif