#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>

#include "ctre_inc.h"

struct pos
{
	int x_;
	int y_;
	int z_;
	auto operator<=>(const pos&) const = default;
	pos(int x, int y, int z) : x_{x}, y_{y}, z_{z}
	{}
};

struct brick
{
	pos l_;
	pos r_;
	brick(pos&& l, pos&& r) : l_{l}, r_{r}
	{}
	brick(int xl, int yl, int zl, int xr, int yr, int zr) : l_{ xl, yl, zl }, r_{ xr, yr, zr }
	{}
};

auto get_input()
{
	std::vector<brick> v;
	std::string ln;
	while (std::getline(std::cin, ln))
	{
		auto [m, xl, yl, zl, xr, yr, zr] = ctre::match<"(\\d+),(\\d+),(\\d+)~(\\d+),(\\d+),(\\d+)">(ln);
		if (m)
			v.emplace_back(xl.to_number<int>(), yl.to_number<int>(), zl.to_number<int>(),
				xr.to_number<int>(), yr.to_number<int>(), zr.to_number<int>());
		else
			std::cout << "TILT { " << ln << " }\n";
	}
	// sort to z
	std::ranges::sort(v, [](auto& l, auto& r) { return l.l_.z_ < r.l_.z_; });

	return v;
}

auto pt1(auto in)
{
	// drop all
	// x,y to min z for this point
	std::map<std::pair<int, int>, int> minz;
	for (auto& b : in)
	{
		int minz_val{ 0 };
		for (int x{ b.l_.x_ }; x <= b.r_.x_; ++x)
		{
			for (int y{ b.l_.y_ }; y <= b.r_.y_; ++y)
			{
				if (minz[{x, y}] >= minz_val)
					minz_val = minz[{x, y}] + 1;
			}
		}
		for (int x{ b.l_.x_ }; x <= b.r_.x_; ++x)
		{
			for (int y{ b.l_.y_ }; y <= b.r_.y_; ++y)
			{
				if (minz[{x, y}] >= minz_val)
					minz_val = minz[{x, y}] + 1;
			}
		}
		b.r_.z_ = minz_val + b.r_.z_ - b.l_.z_;
		b.l_.z_ = minz_val;
	}
	std::cout << "\n";
	for (auto& b : in)
		std::cout << b.l_.x_ << ", " << b.l_.y_ << ", " << b.l_.z_ << " - " << b.r_.x_ << ", " << b.r_.y_ << ", " << b.r_.z_ << "\n";
#if 0
	std::map<pos, size_t> cubes;
	for (size_t n{ 0 }; n < in.size(); ++n)
	{
		auto& b{ in[n] };
		for (int x{ b.l_.x_ }; x <= b.r_.x_; ++x)
			for (int y{ b.l_.y_ }; y <= b.r_.y_; ++y)
				for (int z{ b.l_.z_ }; z <= b.r_.z_; ++z)
					cubes[{x, y, z}] = n;
	}

#endif
	return 0;
}

int main()
{
	auto in{ get_input() };
	std::cout << "got " << in.size() << " bricks.\n";

	std::cout << "pt1 = " << pt1(in) << "\n";
}