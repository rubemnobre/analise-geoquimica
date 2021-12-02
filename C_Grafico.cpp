#include "C_Grafico.hpp"
#include <string>
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <algorithm>

using namespace plot;

C_Grafico::C_Grafico(std::string s, std::string path, std::string plotname){
    study = s;
    folder_path = path;
    name = plotname;
    title = plotname;
    std::replace(title.begin(), title.end(), '_', ' ');
    std::replace(name.begin(), name.end(), ' ', '_');
    std::transform(name.begin(), name.end(), name.begin(), ::tolower);
}

C_Grafico::C_Grafico(){}

void C_Grafico::histogram(){
    command << "set terminal png font \", 9\"\n";
    command << "set style data histograms\n";
    command << "set style fill solid\n";
    command << "set title '" << study << "'\n";
    command << "set output './output/" << study << "-" << name << ".png'\n";
    command << "plot '" << folder_path << "/" << data_fname << "' using 2:xtic(1) title '" << title << "' ls 1\n";
    run("");
}

void C_Grafico::run(std::string opt){
    std::ofstream data_file(folder_path + "/" + data_fname);
    std::ofstream script_file(folder_path + "/" + script_fname);
    data_file << data.rdbuf();
    script_file << command.rdbuf();
    data_file.close();
    script_file.close();
    if(std::system(std::string("gnuplot " + folder_path + "/" + script_fname + " " + opt).c_str()) != 0){
        std::cout << "gnuplot retornou um erro!\n";
    }
}
