#include <iostream>
#include "C_SimuladorTratamentoDados_FT_ICRMS.hpp"

int main(int argc, char **argv){
    std::cout << "Analise Geoquimica - Outubro 2021\n";
    auto sim = C_SimuladorTratamentoDados_FT_ICRMS();
    sim.main_loop();
    return 0;
}