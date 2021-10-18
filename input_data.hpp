#ifndef INPUT_DATA_HPP
#define INPUT_DATA_HPP

#include<string>

namespace input_data{
    class chemical_component{
        public:
            std::string cls;
            std::string mol_formula;
            float intensity;
            int C;
            int H;
            int N;
            chemical_component(char *line_input);
    };
}

#endif