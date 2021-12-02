#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include "C_Amostra.hpp"
#include "C_DBE.hpp"
#include "C_Heteroatomica.hpp"

#ifndef C_GRAFICO_HPP
#define C_GRAFICO_HPP

namespace plot{
    class C_Grafico{
        public:
            C_Grafico();
            C_Grafico(std::string st, std::string path, std::string name);
            void histogram();
            void ylabel(std::string label);
            void xlabel(std::string label);
            std::string title;
            std::stringstream data;
        private:
            std::string name;
            std::string study;
            std::string folder_path;
            std::stringstream command;
            const std::string data_fname = "plot.dat";
            const std::string script_fname = "script";
            void run(std::string opt);
    };
}

#endif