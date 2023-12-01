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

int pt2(auto const& in)
{
	auto to_digit = [](auto const& v)
		{
			switch (v[0])
			{
			case '0':
				return 0;
			case '1':
				return 1;
			case '2':
				return 2;
			case '3':
				return 3;
			case '4':
				return 4;
			case '5':
				return 5;
			case '6':
				return 6;
			case '7':
				return 7;
			case '8':
				return 8;
			case '9':
				return 9;
			case 'o':
				return 1;
			case 't':
				return v[1] == 'w' ? 2 : 3;
			case 'f':
				return v[1] == 'o' ? 4 : 5;
			case 's':
				return v[1] == 'i' ? 6 : 7;
			case 'e':
				return 8;
			case 'n':
				return 9;
			default :
				return 0;
			}
		};

	return std::accumulate(in.begin(), in.end(), 0, [&](auto l, auto& r)
		{
			auto [m, ls] = ctre::search<R"((0|1|2|3|4|5|6|7|8|9|one|two|three|four|five|six|seven|eight|nine))">(r.begin(), r.end());
			auto [m2, rs] = ctre::search<R"((enin|thgie|neves|xis|evif|ruof|eerht|owt|eno|9|8|7|6|5|4|3|2|1|0))">(r.rbegin(), r.rend());
			auto ld = to_digit(ls.to_view());
			auto rsr{ rs.to_string() };
			std::reverse(rsr.begin(), rsr.end());
			auto rd = to_digit(rsr);
			return ld * 10 + rd + l;
		});
}

int main()
{
	auto in { get_input()};
	std::cout << "pt1 = " << pt1(in) << "\n";
	std::cout << "pt2 = " << pt2(in) << "\n";
}
