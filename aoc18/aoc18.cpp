#include <iostream>
#include <string>
#include <vector>
#include <tuple>
#include <algorithm>
#include <numeric>

#include "ctre_inc.h"

using int_t = long long;

struct pt
{
	int_t x_;
	int_t y_;
	auto operator<=>(const pt&) const = default;
};

auto get_input()
{
	std::vector<pt> v1;
	std::vector<pt> v2;
	std::string ln;
	pt p1{ 0, 0 };
	pt p2{ 0, 0 };
	int_t l1{ 0 };
	int_t l2{ 0 };
	v1.push_back(p1);
	v2.push_back(p2);
	while (std::getline(std::cin, ln))
	{
		if (auto [m, d, c] = ctre::match<"([DLRU]) (\\d+) \\(#[\\da-f]{5}[0123]\\)">(ln); m)
		{
			int cnt{ sv_to_t<int>(c, 16) };
			char dir{ d.view()[0]};
			switch (dir)
			{
			case 'R': //right
				p1.x_ += cnt;
				break;
			case 'D': // down
				p1.y_ += cnt;
				break;
			case 'L': // left
				p1.x_ -= cnt;
				break;
			case 'U': // up
				p1.y_ -= cnt;
				break;
			default:
				break;
			}
			v1.push_back(p1);
			l1 += cnt;
		}
		if (auto [m, c, d] = ctre::match<"[DLRU] \\d+ \\(#([\\da-f]{5})([0123])\\)">(ln); m)
		{
			int cnt{ sv_to_t<int>(c, 16) };
			int dir{ sv_to_t<int>(d)};
			switch (dir)
			{
			case 0: //right
				p2.x_ += cnt;
				break;
			case 1: // down
				p2.y_ += cnt;
				break;
			case 2: // left
				p2.x_ -= cnt;
				break;
			case 3: // up
				p2.y_ -= cnt;
				break;
			default:
				break;
			}
			v2.push_back(p2);
			l2 += cnt;
		}
	}
	return std::make_tuple(v1, l1, v2, l2);
}

int_t calculate_area(std::vector<pt> const& v, int_t l)
{
	int_t area{ 0 };
	for (size_t i{ 0 }; i < v.size(); ++i)
	{
		auto j{ i + 1 };
		if (j == v.size())
			j = 0;
		area += v[i].x_ * v[j].y_;
		area -= v[i].y_ * v[j].x_;
	}
	return area / 2 + l/ 2 + 1;
}

auto pt1(auto const& in, int_t l)
{
	return calculate_area(in, l);
}

auto pt2(auto const& in, int_t l)
{
	return calculate_area(in, l);
}

int main()
{
	auto [in1, l1, in2, l2] = get_input();
	std::cout << "pt1 = " << pt1(in1, l1) << "\n";
	std::cout << "pt2 = " << pt2(in2, l2) << "\n";
	std::cout << "(producing the wrong answer for pt1 on actual input!)\n";
}
