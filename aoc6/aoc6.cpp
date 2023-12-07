#include <iostream>
#include <string>
#include <vector>
#include <numeric>
#include <cmath>

#include "ctre_inc.h"

auto get_input()
{
	std::vector<std::pair<int, int>> rv;
	std::string ln;
	std::getline(std::cin, ln);
	for (auto m : ctre::search_all<"(\\d+)">(ln))
		rv.emplace_back(m.to_number<int>(), 0);
	std::getline(std::cin, ln);
	auto i{ rv.begin() };
	for (auto m : ctre::search_all<"(\\d+)">(ln))
		(*i++).second = m.to_number<int>();
	return rv;
}

#if 0
auto count_above(auto tm, auto abv)
{
	decltype(abv) rv{ 0 };
	for (auto t = 0; t < tm; ++t)
	{
		auto d = t * (tm - t);
		if (d > abv)
			++rv;
	}
	return rv;
}
#else
auto count_above(auto tm, auto abv)
{
	auto l{ std::ceil((tm - std::sqrt((tm * tm) - (4 * abv))) / 2.0) };
	auto r{ std::floor((tm + std::sqrt((tm * tm) - (4 * abv))) / 2.0) };
	return static_cast<decltype(tm)>(r - l) + 1;
}
#endif

auto pt1(auto const& in)
{
	int rv{ 1 };
	for (auto& r : in)
		rv *= count_above(r.first, r.second);
	return rv;
}

int factor(int v)
{
	if (v < 10)
		return 10;
	if (v < 100)
		return 100;
	if (v < 1000)
		return 1000;
	if (v < 10000)
		return 10000;
	return 100000; // tilt
}

auto pt2(auto const& in)
{
	long long tm{ 0 };
	long long d{ 0 };
	for (auto& td : in)
	{
		tm *= factor(td.first);
		tm += td.first;
		d *= factor(td.second);
		d += td.second;
	}
	return count_above(tm, d);
}

int main()
{
	auto in{ get_input() };
	std::cout << "pt1 = " << pt1(in) << "\n";
	std::cout << "pt2 = " << pt2(in) << "\n";
}