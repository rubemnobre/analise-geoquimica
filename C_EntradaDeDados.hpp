#include <fstream>
#include <string>
#include "C_Amostra.hpp"
#include "C_ComponenteQuimico.hpp"

#ifndef C_ENTRADA_DE_DADOS_HPP
#define C_ENTRADA_DE_DADOS_HPP

namespace data{
    class C_EntradaDeDados{
        public:
            C_EntradaDeDados();
            bool open(std::string fname);
            C_Amostra* get_sample();
            bool is_successful();
            std::string error = "";
        private:
            component_vector components;
            bool success = false;
            std::string filename;
    };
    
    std::vector<std::string> split_line(char *str);
    
    component_vector read_ifstream(std::ifstream *input_data);
}

#endif