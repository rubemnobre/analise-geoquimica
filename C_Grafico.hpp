#include <vector>
#include <string>
#include <sstream>
#include "C_Amostra.hpp"
#include "C_DBE.hpp"
#include "C_Heteroatomica.hpp"

#ifndef C_GRAFICO_HPP
#define C_GRAFICO_HPP

namespace plot{
    class C_Grafico{
        public:
            C_Grafico();
            void histogram();
            void histogram(std::string fname);
        private:
            std::stringstream data_text;
            std::stringstream command;
    };
}

#endif