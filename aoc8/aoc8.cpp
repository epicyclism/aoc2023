#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <numeric>

#include "ctre_inc.h"

auto get_input()
{
	std::string dir;
	std::getline(std::cin, dir);
	std::string ln;
	std::map<std::string, std::pair<std::string, std::string>> mp;
	while(std::getline(std::cin, ln))
	{
		if(auto[m, f, l, r] = ctre::match<"(\\w+) = \\((\\w+), (\\w+)\\)">(ln); m)
			mp.emplace(f.to_string(), std::make_pair(l.to_string(), r.to_string()));
	}
	return std::make_pair(dir, mp);
}

auto pt1(auto const& in)
{
	auto const& dir { in.first};
	auto const& mp  { in.second};
	int steps{0};
	size_t d{ 0};
	auto it { mp.find("AAA")};
	while((*it).first != "ZZZ")
	{
		if(dir[d] == 'L')
			it = mp.find((*it).second.first);
		else
			it = mp.find((*it).second.second);
		++steps;
		++d;
		if(d == dir.length())
			d = 0;
	}
	return steps;
}

auto pt2(auto const& in)
{
	auto const& dir { in.first};
	auto const& mp  { in.second};
	int steps{0};
	size_t d{ 0};
	std::vector<decltype(mp.begin())> its;
	for(auto i{mp.begin()}; i != mp.end(); ++i)
	{
		if((*i).first.back() == 'A')
			its.push_back(i);
	}
	std::vector<long long> vm;
	for(auto i : its)
	{
		int steps{0};
		size_t d{ 0};
		while((*i).first.back() != 'Z')
		{
			if(dir[d] == 'L')
				i = mp.find((*i).second.first);
			else
				i = mp.find((*i).second.second);
			++steps;
			++d;
			if(d == dir.length())
				d = 0;
		}
		vm.push_back(steps);
	}
	return std::reduce(vm.begin(), vm.end(), 1LL, [](auto l, auto r){return std::lcm(l, r);});
}

int main()
{
	auto in { get_input()};
	std::cout << "pt1 = " << pt1(in) << "\n";
	std::cout << "pt2 = " << pt2(in) << "\n";
}
