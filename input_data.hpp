#ifndef INPUT_DATA_HPP
#define INPUT_DATA_HPP

#include <fstream>
#include <string>
#include <vector>
#include <map>
#define N_COLS 7

namespace data{
    class C_ComponenteQuimico{ // Armazena os dados relevantes de cada componente
        public:
            std::string cls;
            std::string mol_formula;
            float intensity;
            int C;
            int H;
            int N;
            int DBE;
            C_ComponenteQuimico(std::vector<std::string> input_cols); // Obtem os dados de um vetor com as strings de cada coluna de cada linha da entrada
            std::string to_line(); // Imprime os dados em uma string para a saida
    };

    typedef std::vector<C_ComponenteQuimico> component_vector;

    std::vector<std::string> split_line(char *str);

    std::vector<std::string> get_classes(component_vector components);

    component_vector read_ifstream(std::ifstream *input_data);

    component_vector components_per_class(component_vector components, std::string cls); // Retorna os componentes da classe especificada por cls

    void write_modified(component_vector components, std::ofstream *file); // Escreve os dados relevantes do vetor na stream apontada por file
    
    float total_class_intensity(component_vector components, std::string cls);

    struct intensity{ // Para ter um valor padrão no mapa
        float x = 0;
    };

    class C_DBE{ // Armazena os dados relevantes de um grupo de componentes com o mesmo C_DBE
        public:
            component_vector dbe_components;
            std::string parent_class = "";
            int val;
            float intensity = 0;
            std::map<int, struct intensity> c_intensity;
            int min_c = 10000; // Guardara o C do componente com o menor C no C_DBE
            int max_c = -1; // Guardara o C do componente com o maior C no C_DBE
            float sum_c(int min, int max, int opt);
            float sum_c(std::vector<int> vals);
            C_DBE(component_vector components, int C_DBE);
            void print_intensity_per_c(std::ostream &output);

            // A ser usado na variavel opt da funcao sum_c para escolher entre somar qualquer C, os Impares e os Pares
            static const int all = 0; 
            static const int odd = 1;
            static const int even = 2;
    };

    class C_Heteroatomica{ // Armazena os dados importantes de uma classe heteroatomica
        public:
            component_vector class_components;
            std::string class_name;
            float intensity = 0;
            std::map<int, C_DBE*> class_dbes;
            float sum_dbe(int min, int max);
            float sum_dbe(std::vector<int> indices);
            C_Heteroatomica(component_vector components, std::string name);
            C_DBE *get_DBE(int C_DBE); // Retorna um ponteiro ao C_DBE especificado. Se não existir, inicializa um objeto novo e retorna
            float C(int C_DBE, int c);

            // Chamam a funcao sum_c do C_DBE, simplifica a sintaxe de chamar a funcao do ponteiro dentro do mapa
            float sum_C(int C_DBE, int min, int max, int opt); 
            float sum_C(int C_DBE, std::vector<int> vals);

            void print_intensity_per_dbe(std::ostream &output);
    };

    class C_Amostra{ // Armazena os dados refinados de uma amostra completa
        public:
            float intensity;
            component_vector C_Amostra_components;
            std::map<std::string, C_Heteroatomica*> classes;
            std::map<int, C_DBE*> dbes;
            std::vector<std::string> class_names;
            C_Amostra(component_vector components);
            C_Heteroatomica *get_class(std::string name); // Se não existir, inicializa um objeto novo e retorna
            C_DBE *get_DBE(int C_DBE); // Se nao existir, inicializa um objeto novo e retorna
            void print_relative_abundancy(std::ostream &output);
            void print_biodegradation(std::ostream &output);
            void print_paleoenvironment(std::ostream &output);
            void print_maturity(std::ostream &output);
    };

    class C_SaidaDeDados{
        public:
            std::string study_name = "default";
            std::string folder_path = "./";
            C_SaidaDeDados(std::string name, std::string path);
            std::ofstream new_output(std::string name);
    };
}

#endif