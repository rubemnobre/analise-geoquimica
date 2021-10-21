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

    typedef std::vector<chemical_component> component_vector;

    component_vector read_ifstream(std::ifstream *input_data);

    std::vector<std::string> split_line(char *str);

    void write_modified(component_vector components, std::ofstream *file);
}

namespace analysis{
    std::vector<std::string> get_classes(data::component_vector components);

    float total_class_intensity(data::component_vector components, std::string cls);

    data::component_vector components_per_class(data::component_vector components, std::string cls);
}

#endif