#ifndef INPUT_DATA_HPP
#define INPUT_DATA_HPP

#include <fstream>
#include <string>
#include <vector>
#include <map>
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

    class heteroatomic_class{
        public:
            component_vector class_components;
            std::string class_name;
            float total_intensity;
            std::map<int, float> dbe_intensity;
            heteroatomic_class(component_vector class_components);
            heteroatomic_class(component_vector components, std::string class_name);
        private:
            void construct(component_vector class_components);
    };

    std::vector<std::string> get_classes(component_vector components);

    float total_class_intensity(component_vector components, std::string cls);

    component_vector components_per_class(component_vector components, std::string cls);
}

#endif