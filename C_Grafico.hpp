#include <vector>
#include <string>

#ifndef C_GRAFICO_HPP
#define C_GRAFICO_HPP

namespace plot{
    template <typename xtype> class C_Grafico{
        public:
            C_Grafico(const int type);
            void add_point(xtype x, float y);
            void add_point(xtype x, float y, std::string group);
            void add_group(std::vector<xtype> x, std::vector<float> y, std::string label);
            void plot(std::string fname);
            static const int histogram = 0;
            static const int scatter = 1;
    };
}

#endif