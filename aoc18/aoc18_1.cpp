#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <algorithm>

#define MDSPAN_USE_PAREN_OPERATOR 1
#include <experimental/mdspan>
namespace stdex = std::experimental;

#include "ctre_inc.h"

struct pt
{
	int x_;
	int y_;
	auto operator<=>(const pt&) const = default;
};

auto get_input()
{
	std::vector<pt> v;
	std::string ln;
	int x{ 0 };
	int y{ 0 };
	bool bud{ false };
	while (std::getline(std::cin, ln))
	{
		if (auto [m, d, c, col] = ctre::match<"([DLRU]) (\\d+) \\(#([\\da-f]+)\\)">(ln); m)
		{
			int val{ c.to_number<int>()};
			switch (d.view()[0])
			{
			case 'D':
				for (int cnt{ 0 }; cnt < val; ++cnt)
					v.push_back({ x, y++ });
				if (bud)
					std::cout << "bud at " << ln << "\n";
				bud = true;
				break;
			case 'L':
				for (int cnt{ 0 }; cnt < val; ++cnt)
					v.push_back({ x--, y });
				bud = false;
				break;
			case 'R':
				for (int cnt{ 0 }; cnt < val; ++cnt)
					v.push_back({ x++, y });
				bud = false;
				break;
			case 'U':
				for (int cnt{ 0 }; cnt < val; ++cnt)
					v.push_back({ x, y-- });
				if (bud)
					std::cout << "bud at " << ln << "\n";
				bud = true;
				break;
			default:
				break;
			}
		}
	}
	auto [minx, maxx] = std::minmax_element(v.begin(), v.end(), [](auto& l, auto& r) { return l.x_ < r.x_; });
	auto [miny, maxy] = std::minmax_element(v.begin(), v.end(), [](auto& l, auto& r) { return l.y_ < r.y_; });
	std::cout << "x " << (*minx).x_ << " - " << (*maxx).x_ << "\n";
	std::cout << "y " << (*miny).y_ << " - " << (*maxy).y_ << "\n";
	int offx = -(*minx).x_;
	int offy = -(*miny).y_;
	std::cout << "offset x = " << offx << ", y = " << offy << "\n";
	std::transform(v.begin(), v.end(), v.begin(), [&](auto& p) -> pt { return { p.x_ + offx, p.y_ + offy }; });
	return v;
}

auto pt0(auto in)
{
	std::ranges::sort(in);
	size_t in_cnt{ 0 };
	auto mmx{ std::minmax_element(in.begin(), in.end(), [](auto& l, auto& r) { return l.x_ < r.x_; }) };
	auto mmy{ std::minmax_element(in.begin(), in.end(), [](auto& l, auto& r) { return l.y_ < r.y_; }) };
	std::cout << "x " << (*mmx.first).x_ << " - " << (*mmx.second).x_ << "\n";
	std::cout << "y " << (*mmy.first).y_ << " - " << (*mmy.second).y_ << "\n";
	auto i{ in.begin() };
	auto ii{ std::adjacent_find(in.begin(), in.end(), [](auto& l, auto& r) { return l.x_ != r.x_; }) };
	while ( ii != in.end())
	{
		++ii;
		while (i != ii)
		{
			++i;
		}
		ii = std::adjacent_find(ii, in.end(), [](auto& l, auto& r) { return l.x_ != r.x_; });
	}
	return in.size() + in_cnt;
}

void flood_fill(auto md)
{
	pt p{ 0, 0 };
	std::queue<pt> q;
	q.push(p);
	while (!q.empty())
	{
		auto n{ q.front() };
		q.pop();
		if (md(n.y_, n.x_) == '.')
		{
			md(n.y_, n.x_) = '*';
			if (n.y_ > 0)
				q.push({ n.x_, n.y_ - 1 });
			if (n.x_ < md.extent(1) - 1)
				q.push({ n.x_ + 1, n.y_ });
			if (n.y_ < md.extent(0) - 1)
				q.push({ n.x_, n.y_ + 1 });
			if (n.x_ > 0)
				q.push({ n.x_ - 1, n.y_ });
		}
	}
}

void print(auto md)
{
	std::cout << "\n";
	for (size_t y{ 0 }; y < md.extent(0); ++y)
	{
		for (size_t x{ 0 }; x < md.extent(1); ++x)
		{
			std::cout << md(y, x);
		}
		std::cout << "\n";
	}
}

auto pt1(auto const& in)
{
	auto mx = (*std::max_element(in.begin(), in.end(), [](auto& l, auto& r) { return l.x_ < r.x_; })).x_;
	auto my = (*std::max_element(in.begin(), in.end(), [](auto& l, auto& r) { return l.y_ < r.y_; })).y_;

	std::vector<char> v((mx + 3) * (my + 3), '.');
	stdex::mdspan md(v.data(), my + 3, mx + 3);
	for (auto& p : in)
		md(p.y_ + 1, p.x_ + 1) = '#';
//	print(md);
	flood_fill(md);
//	print(md);
	std::cout << "v1 = " << std::count(v.begin(), v.end(), '.') + std::count(v.begin(), v.end(), '#') << "\n";
	return v.size() - std::count(v.begin(), v.end(), '*');
}

int main()
{
	auto in{ get_input() };
	std::cout << "got " << in.size() << " pts.\n";
	std::cout << "pt1 = " << pt1(in) << "\n";
}

// 22267 too low