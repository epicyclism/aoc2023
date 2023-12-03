#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#define MDSPAN_USE_PAREN_OPERATOR 1
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
   v.insert(v.end(), stride, '.');

   return std::make_pair(stride, v);
}

struct num
{
    size_t xf_;
    size_t xt_;
    size_t y_;
    int val_;
};

void to_value(auto const& md, auto& nm)
{
    int rv { 0 };
    for(auto i { nm.xf_}; i < nm.xt_; ++i)
    {
        rv *= 10;
        rv += md(nm.y_, i) - '0';
    }
    nm.val_ = rv;
}

auto get_numbers(auto const& in)
{
    size_t stride { in.first};
    size_t height{ in.second.size() / in.first};
    stdex::mdspan md(in.second.data(), stride, height);
    std::vector<num> vn;
    for(size_t y { 0 }; y < height; ++y)
    {
        bool inn { false };
        num nm { 0, 0, y, 0};
        for(size_t x { 0 }; x < stride; ++x)
        {
            if(::isdigit(md(y, x)))
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
                    to_value(md, nm);
                    vn.emplace_back(std::move(nm));
                }
            }
        }
    }
    return vn;
}

bool is_sym(char c)
{
    return !(::isdigit(c) || c == '.');
}

bool is_part_num(auto const& md, auto const& nm)
{
    // along the top
    for(auto i { nm.xf_ - 1}; i < nm.xt_ + 1; ++i)
        if(is_sym(md(nm.y_ - 1, i)))
            return true;
    // sides
    if(is_sym(md(nm.y_, nm.xf_ - 1)))
        return true;
    if(is_sym(md(nm.y_, nm.xt_)))
        return true;
    // along the bottom
    for(auto i { nm.xf_ - 1}; i < nm.xt_ + 1; ++i)
        if(is_sym(md(nm.y_ + 1, i)))
            return true;

    return false;
}

auto pt1(auto const& in)
{
    auto vn{get_numbers(in)};
    size_t stride { in.first};
    size_t height{ in.second.size() / in.first};
    stdex::mdspan md(in.second.data(), stride, height);

    int rv { 0};
    for(auto& nm: vn)
        if(is_part_num(md, nm))
            rv += nm.val_;

    return rv;
}

struct gear
{
    size_t x_;
    size_t y_;
    int val_;
};

void find_gear(auto const& md, auto const& nm, auto& vg)
{
    // along the top
    for(size_t i { nm.xf_ - 1}; i < nm.xt_ + 1; ++i)
        if(md(nm.y_ - 1, i) == '*')
            vg.push_back({i, nm.y_ - 1, nm.val_});
    // sides
    if(md(nm.y_, nm.xf_ - 1) == '*')
        vg.push_back({nm.xf_ - 1, nm.y_, nm.val_});
    if(md(nm.y_, nm.xt_) == '*')
        vg.push_back({nm.xt_, nm.y_, nm.val_});
    // along the bottom
    for(size_t i { nm.xf_ - 1}; i < nm.xt_ + 1; ++i)
        if(md(nm.y_ + 1, i) == '*')
            vg.push_back({i, nm.y_ + 1, nm.val_});
}

auto pt2(auto const& in)
{
    auto vn{get_numbers(in)};
    size_t stride { in.first};
    size_t height{ in.second.size() / in.first};
    stdex::mdspan md(in.second.data(), stride, height);

    std::vector<gear> vg;
    for(auto& nm: vn)
        find_gear(md, nm, vg);

    std::ranges::sort(vg, [](auto& l, auto& r){ return l.x_ == r.x_ ? l.y_ < r.y_ : l.x_ < r.x_;});

//    for(auto& g: vg)
//        std::cout << g.x_ << ", " << g.y_ << " = " << g.val_ << "\n";

    int rv { 0};
    auto i1 { std::adjacent_find(vg.begin(), vg.end(), [](auto& l, auto& r) { return l.x_ == r.x_ && l.y_ == r.y_;})};
    while(i1 != vg.end())
    {
        auto i2 { i1 + 1};
        rv += (*i1).val_ * (*i2).val_;
        i1 = std::adjacent_find(i2, vg.end(), [](auto& l, auto& r) { return l.x_ == r.x_ && l.y_ == r.y_;});
    }
    return rv;
}

int main()
{
   auto in{get_input()};
   int cnt{0};
   std::cout << "pt1 = " << pt1 (in) << "\n";
   std::cout << "pt2 = " << pt2 (in) << "\n";
}
