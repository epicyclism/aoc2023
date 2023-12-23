#include <iostream>
#include <string>
#include <vector>
#include <queue>
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
	std::ranges::sort(v, [](auto& l, auto& r) { return l.l_.z_ < r.l_.z_; });

	return v;
}

auto pt12(auto in)
{
	// drop all
	// x,y to min z for this point
	std::map<std::pair<int, int>, int> minz;
	for (auto& b : in)
	{
		int minz_val{ 1 };
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
		for (int x{ b.l_.x_ }; x <= b.r_.x_; ++x)
			for (int y{ b.l_.y_ }; y <= b.r_.y_; ++y)
				 minz[{x, y}] = b.r_.z_ ;
	}
	// mark all occupied cubes with brick id
	std::map<pos, size_t> cubes;
	for (size_t n{ 0 }; n < in.size(); ++n)
	{
		auto& b{ in[n] };
		for (int x{ b.l_.x_ }; x <= b.r_.x_; ++x)
			for (int y{ b.l_.y_ }; y <= b.r_.y_; ++y)
				for (int z{ b.l_.z_ }; z <= b.r_.z_; ++z)
					cubes[{x, y, z}] = n;
	}
	// for each brick find bricks it supports
	std::vector<std::vector<size_t>> supports(in.size());
	std::vector<std::vector<size_t>> supportedby(in.size());
	size_t ind{ 0 };
	for (auto& b : in)
	{
		int z_up{ b.r_.z_ + 1 };
		for(int x { b.l_.x_}; x <= b.r_.x_; ++x)
			for (int y{ b.l_.y_ }; y <= b.r_.y_; ++y)
			{
				auto i = cubes.find({ x, y, z_up });
				if (i != cubes.end())
				{
					supports[ind].push_back((*i).second);
					supportedby[(*i).second].push_back( ind);
				}
			}
		++ind;
	}
	for (auto& s : supports)
	{
		std::ranges::sort(s);
		auto ne = std::unique(s.begin(), s.end());
		s.erase(ne, s.end());
	}
	for (auto& s : supportedby)
	{
		std::ranges::sort(s);
		auto ne = std::unique(s.begin(), s.end());
		s.erase(ne, s.end());
	}
#if 0
	for (int n{ 0 }; n < in.size(); ++n)
	{
		std::cout << "block " << n << " supports ";
		for (auto bs : supports[n])
			std::cout << bs << " ";
		std::cout << " and is supported by ";
		for (auto bs : supportedby[n])
			std::cout << bs << " ";
		std::cout << "\n";
	}
#endif
	int can_disintegrate{ 0 };
	for (int n{ 0 }; n < in.size(); ++n)
	{
		bool can_d{ true };
		for (auto under : supports[n])
			if (supportedby[under].size() == 1)
				can_d = false;
		can_disintegrate += can_d;
	}
	int64_t cnt_other{ 0 };
	for (int n{ 0 }; n < in.size(); ++n)
	{
		auto lsupports{ supports };
		auto lsupportedby{ supportedby };
		std::queue<int> q;
		q.push(n);
		while(!q.empty())
		{ 
			auto b = q.front();
			q.pop();
			for (auto bs : lsupports[b])
			{
				std::erase(lsupportedby[bs], b);
				if(lsupportedby[bs].empty())
				{
					q.push(bs);
					++cnt_other;
				}
			}
		}
//		std::cout << n << " - ";
//		for (auto& v : lsupportedby)
//			std::cout << v.size() << " ";
//		std::cout << "\n";
//		cnt_other += std::count_if(lsupportedby.begin() + 1, lsupportedby.end(), [](auto& v) { return v.empty(); }) ;
	}
	return std::make_pair(can_disintegrate, cnt_other);
}

int main()
{
	auto in{ get_input() };
	std::cout << "got " << in.size() << " bricks.\n";

	auto [pt1, pt2] = pt12(in);
	std::cout << "pt1 = " << pt1 << "\n";
	std::cout << "pt2 = " << pt2 << "\n";
}
// 128505 too high