#include <iostream>
#include <string>
#include <vector>

#include <experimental/mdspan>
namespace stdex = std::experimental;

auto get_input()
{
    std::vector<char> v;
    std::string ln;
    std::getline(std::cin, ln);
    auto stride {ln.size() + 2};
    v.insert(v.begin(), stride, '.');
    do
    {
        v.push_back('.');
        v.insert(v.end(), ln.begin(), ln.end());
        v.push_back('.');
    } while (std::getline(std::cin, ln));
   v.insert(v.begin(), stride, '.');

   return std::make_pair(stride, v);
}

struct num
{
    size_t xf_;
    size_t xt_;
    size_t y_;
};

auto pt1(auto const& in)
{
    stdex::mdspan md(in.second.data(), in.first, in.second.size() / in.first);
    std::vector<num> vn;
    for(size_t y { 0 }; y < md.stride(1); ++y)
    {
        bool inn { false };
        num nm { 0, 0, y};
        for(size_t x { 0 }; x < md.stride(0); ++x)
        {
            if(::isdigit(md(x, y)))
            {
                if(!inn)
                {
                    inn = true;
                    nm.xf_ = x;
                }
            }
            else
            {
                if(inn)
                {
                    inn = false;
                    nm.xt_ = x;
                    vn.emplace_back(std::move(nm));
                }
            }
        }
    }
    return vn.size();
}

int main()
{
   auto in{get_input()};
   std::cout << "pt1 = " << pt1 (in) << "\n";
}
