#include "C_DistC.hpp"

data::C_DistC::C_DistC(){}

void data::C_DistC::append(C_ComponenteQuimico i){
    c_intensity[i.C].x += i.intensity;
    if(i.C > max_c) max_c = i.C;
    if(i.C < min_c) min_c = i.C;
}

float data::C_DistC::sum_c(int min, int max, int opt){
    float sum = 0.0;
    if(opt == all) for(int i = min; i <= max; i++) sum += c_intensity[i].x;

    if(opt == even) for(int i = min%2?min+1:min; i <= max; i += 2) sum += c_intensity[i].x;

    if(opt == odd) for(int i = min%2?min:min+1; i <= max; i += 2) sum += c_intensity[i].x;

    return sum;
}

float data::C_DistC::sum_c(std::vector<int> vals){
    float sum = 0.0;
    for(auto i : vals) sum += c_intensity[i].x;
    return sum;
}

float &data::C_DistC::operator[](int i){
    return c_intensity[i].x;
}