#include <string>

#ifndef C_SAIDA_DE_DADOS_HPP
#define C_SAIDA_DE_DADOS_HPP

namespace data{
    class C_SaidaDeDados{
        public:
            std::string study_name = "default";
            std::string folder_path = "./";
            C_SaidaDeDados(std::string name, std::string path);
            std::ofstream new_output(std::string name);
    };
}

#endif