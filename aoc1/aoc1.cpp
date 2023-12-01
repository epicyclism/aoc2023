#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <numeric>

#include "ctre_inc.h"

auto get_input()
{
	std::vector<std::string> vin;
	std::string ln;
	while(std::getline(std::cin, ln))
		vin.emplace_back(ln);
	return vin;
}

int pt1(auto const& in)
{
	return std::accumulate(in.begin(), in.end(), 0, [](auto l, auto& r)
		{
			int ld { r[r.find_first_of("0123456789")] - '0'};
			int rd { r[r.find_last_of("0123456789")] - '0'};
			return l + ld * 10 + rd;
		});
}

template<typename I> int to_digit(I b)
{
	if (::isdigit(*b))
		return *b - '0';
	if (*b == 'o')
		return 1;
	if( *b == 't')
		return *(b + 1) == 'w' ? 2 : 3;
	if( *b == 'f')
		return *(b + 1) == 'o' ? 4 : 5;
	if( *b == 's')
		return *(b + 1) == 'i' ? 6 : 7;
	if (*b == 'e')
		return 8;
	if(*b == 'n')
		return 9;
	return -10000000;
}

int pt2(auto const& in)
{
	return std::accumulate(in.begin(), in.end(), 0, [&](auto l, auto& r)
		{
			auto [m, ls] = ctre::search<R"((\d|one|two|three|four|five|six|seven|eight|nine))">(r.begin(), r.end());
			auto [m2, rs] = ctre::search<R"((enin|thgie|neves|xis|evif|ruof|eerht|owt|eno|\d))">(r.rbegin(), r.rend());
			auto vl{ ls.view() };
			auto ld = to_digit( vl.begin());
			auto vr{ rs.to_string() };
			auto rd = to_digit(vr.rbegin());
			return ld * 10 + rd + l;
		});
}

int main()
{
	auto in { get_input()};
	std::cout << "pt1 = " << pt1(in) << "\n";
	std::cout << "pt2 = " << pt2(in) << "\n";
}
