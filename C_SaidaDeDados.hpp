#include <string>
#include "C_Grafico.hpp"

#ifndef C_SAIDA_DE_DADOS_HPP
#define C_SAIDA_DE_DADOS_HPP

namespace data{
    class C_SaidaDeDados{
        public:
            std::string study_name = "default";
            std::string folder_path = "./";
            C_SaidaDeDados(std::string name, std::string path);
            std::ofstream new_output(std::string name);
            plot::C_Grafico new_plot(std::string name);
    };
}

#endif