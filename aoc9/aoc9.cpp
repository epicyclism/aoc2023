#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>

#include "ctre_inc.h"

auto get_input()
{
	std::vector<std::vector<int>> vv;
	std::string ln;
	while(std::getline(std::cin, ln))
	{
		std::vector<int> v;
		for(auto m : ctre::search_all<"-?\\d+">(ln))
			v.push_back(m.to_number<int>());
		vv.emplace_back(std::move(v));
	}
	return vv;
}

template<typename CI, typename CO> void my_adj_diff(CI const& ci, CO& co)
{
	if(ci.empty())
		return;
	auto b { ci.begin()};
	auto bn { b + 1};
	auto e { ci.end()};
	while(bn != e)
		co.push_back((*bn++) - (*b++));
}

int next_val(std::vector<int> const& v)
{
	if(std::ranges::all_of(v, [](auto i){return i == 0;}))
		return 0;
	std::vector<int> v2;
	v2.reserve(v.size());
	my_adj_diff(v, v2);
	v2.push_back(v2.back() + next_val(v2));

	return v2.back();
}

auto pt1(auto const& in)
{
	return std::accumulate(in.begin(), in.end(), 0, [](auto l, auto& r){ return l + r.back() + next_val(r);});
}

auto pt2(auto in)
{
	for(auto& v: in)
		std::ranges::reverse(v);
	return pt1(in);
}

int main()
{
	auto in{ get_input()};

	std::cout << "pt1 = " << pt1(in) << "\n";
	std::cout << "pt2 = " << pt2(in) << "\n";
}