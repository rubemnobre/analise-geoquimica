#include <fstream>
#include <string>
#include "C_ComponenteQuimico.hpp"
#include "C_Amostra.hpp"
#include "C_SaidaDeDados.hpp"

class C_SimuladorTratamentoDados_FT_ICRMS{
    public:
        C_SimuladorTratamentoDados_FT_ICRMS();
        void main_loop();
        void relative_abundancy(data::C_Amostra input, data::C_SaidaDeDados out);
        void petrochemical_study(data::C_Amostra input, data::C_SaidaDeDados out);
        void geochemical_study(data::C_Amostra input, data::C_SaidaDeDados out);
    private:
        std::ifstream input_file;
        std::string nome;
        data::component_vector components;
        data::C_Amostra *input;
        std::string study_name;
        data::C_SaidaDeDados *out;
};