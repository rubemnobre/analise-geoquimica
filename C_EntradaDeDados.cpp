#include "C_EntradaDeDados.hpp"

#define MAX_WIDTH 100
#define N_COLS 7

std::vector<std::string> data::split_line(char *str){
    std::vector<std::string> output;
    int last = 0;
    int i_col = 0;
    for(int i = 0; str[i] != '\0' && i_col < N_COLS - 1; i++){
        if(isspace(str[i])){
            do{
                i++;
            }while(isspace(str[i]));
            output.push_back(std::string(&str[last], (i - 1) - last));
            i_col++;
            last = i;
        }
    }
    output.push_back(std::string(&str[last]));
    return output;
}

data::component_vector data::read_ifstream(std::ifstream *input){
    data::component_vector data_vector;
    char header[MAX_WIDTH];
    input->getline(header, MAX_WIDTH);
    while(!input->eof()){
        char input_line[MAX_WIDTH];
        input->getline(input_line, MAX_WIDTH);
        auto input_cols = data::split_line(input_line);
        if(input_cols.size() == 7){
            data_vector.push_back(data::C_ComponenteQuimico(input_cols));
        }
    }
    return data_vector;
}

bool data::C_EntradaDeDados::open(std::string fname){
    error = "";
    success = false;
    auto input_file = std::ifstream(fname, std::ifstream::in);
    if(input_file.is_open()){
        components = data::read_ifstream(&input_file);
        input_file.close();
        if(components.size() <= 0){
            error += "Nenhum componente lido\n";
        }else{
            success = true;
        }
    }else{
        error += "Erro abrindo o arquivo\n";
    }
    return success;
}

data::C_EntradaDeDados::C_EntradaDeDados(){}

bool data::C_EntradaDeDados::is_successful(){
    return success;
}

data::C_Amostra* data::C_EntradaDeDados::get_sample(){
    return new data::C_Amostra(components);
}