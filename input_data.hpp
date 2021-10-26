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

    std::vector<std::string> split_line(char *str);

    std::vector<std::string> get_classes(component_vector components);

    component_vector read_ifstream(std::ifstream *input_data);

    component_vector components_per_class(component_vector components, std::string cls);

    void write_modified(component_vector components, std::ofstream *file);
    
    float total_class_intensity(component_vector components, std::string cls);

    struct intensity{
        float x = 0;
    };

    class DBE{
        public:
            component_vector dbe_components;
            int val;
            float intensity = 0;
            std::map<int, struct intensity> c_intensity;
            int min_c = 10000;
            int max_c = -1;
            float sum_c(int min, int max, int opt);
            float sum_c(std::vector<int> vals);
            DBE(component_vector components, int dbe);
            static const int all = 0;
            static const int odd = 1;
            static const int even = 2;
    };

    class heteroatomic_class{
        public:
            component_vector class_components;
            std::string class_name;
            float intensity = 0;
            std::map<int, DBE*> class_dbes;
            float sum_dbe(int min, int max);
            float sum_dbe(std::vector<int> indices);
            heteroatomic_class(component_vector components, std::string name);
            DBE *get_DBE(int dbe);
            float C(int dbe, int c);
            float sum_C(int dbe, int min, int max, int opt);
            float sum_C(int dbe, std::vector<int> vals);
    };

    class sample{
        public:
            float intensity;
            component_vector sample_components;
            std::map<std::string, heteroatomic_class*> classes;
            std::map<int, DBE*> dbes;
            std::vector<std::string> class_names;
            sample(component_vector components);
            heteroatomic_class *get_class(std::string name);
            DBE *get_DBE(int dbe);
    };
}

#endif