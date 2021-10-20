#ifndef INPUT_DATA_HPP
#define INPUT_DATA_HPP

#include <fstream>
#include <string>
#include <vector>
#define N_COLS 7

namespace data{
    class chemical_component{
        public:
            std::string cls;
            std::string mol_formula;
            float intensity;
            int C;
            int H;
            int N;
            int DBE;
            chemical_component(std::vector<std::string> input_cols);
            std::string to_line();
    };

    std::vector<chemical_component> read_ifstream(std::ifstream *input_data);

    std::vector<std::string> split_line(char *str);

    void write_modified(std::vector<chemical_component> components, std::ofstream *file);
}

namespace analysis{
    std::vector<std::string> get_classes(std::vector<data::chemical_component> components);

    float total_class_intensity(std::vector<data::chemical_component> components, std::string cls);
}

#endif