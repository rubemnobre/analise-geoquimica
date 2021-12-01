#include <string>
#include <fstream>
#include "C_SaidaDeDados.hpp"
#include "C_ComponenteQuimico.hpp"

data::C_SaidaDeDados::C_SaidaDeDados(std::string name, std::string folder){
    study_name = name;
    folder_path = folder;
}

std::ofstream data::C_SaidaDeDados::new_output(std::string name){
    return std::ofstream(folder_path + "/" + study_name + "_" + name + ".txt", std::ofstream::out | std::ofstream::app);
}