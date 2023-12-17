#include <iostream>
#include <string>
#include <vector>
#include <iterator>
#include <limits>
#include <queue>
#include <map>
#include <algorithm>
#include <numeric>

#define MDSPAN_USE_PAREN_OPERATOR 1
#include <experimental/mdspan>
namespace stdex = std::experimental;

auto get_input()
{
	std::vector<char> v;
	int s{ 0 };
	std::string ln;
	while (std::getline(std::cin, ln))
	{
		std::transform(ln.begin(), ln.end(), std::back_inserter(v), [](auto c) { return c - '0'; });
		if (!ln.empty())
			s = ln.size();
	}
	return std::make_pair(v, s);
}

// N = 0, E = 1, S = 2, W = 3
char left(char d)
{
	--d;
	if (d < 0)
		d += 4;
	return d;
}

char right(char d)
{
	++d;
	if (d > 3)
		d -= 4;
	return d;
}

auto move(int x, int y, char d) -> std::pair<int, int>
{
	switch (d)
	{
	case 0:
		return { x, y - 1 };
	case 1:
		return { x + 1, y };
	case 2:
		return { x, y + 1 };
	case 3:
		return { x - 1, y };
	}
	std::cout << "TILT\n";
	return { -1, -1 };
}

bool valid_move(int x, int y, auto const& md)
{
	return (x >= 0 && x < int(md.extent(1)) && y >= 0 && y < int(md.extent(0)));
}

struct path
{
	int x_;
	int y_;
	char d_;
	char steps_;
	auto operator<=>(const path&) const = default;

//	path() : x_{ 0 }, y_{ 0 }, d_{ 0 }, steps_{ 0 }
//	{}
//	path(int sc, char st, char d) : x_{ 0 }, y_{ 0 }, steps_ { st }, d_{ d }
//	{}
};

auto pt1(auto const& in)
{
	auto const& v{ in.first };
	auto s{ in.second };
	stdex::mdspan md(v.data(), v.size() / s, s);
	int xe{ int(md.extent(1)) - 1 };
	int ye{ int(md.extent(0)) - 1 };
	int mn{ std::numeric_limits<int>::max() };
	std::map<path, int> cache;
	std::queue<std::pair<path, int>> q;
	q.push({ { 0, 0, 1, 0 }, 0 }); // East
	while (!q.empty())
	{
		auto ps{ q.front() };
		q.pop();
		auto& p{ ps.first };
		auto sc{ ps.second };
		auto ic{ cache.find(p) };
		if (ic != cache.end() && (*ic).second < sc)
			continue;

		if (p.x_ == xe && p.y_ == ye)
		{
			if (sc < mn)
				mn = sc;
		}
		else
		{
			// left
			auto xy = move(p.x_, p.y_, left(p.d_));
			if (valid_move(xy.first, xy.second, md))
			{
				path np{ xy.first, xy.second, left(p.d_), 1 };
				auto ns{ sc + md(xy.second, xy.first) };
				auto ic{ cache.find(np) };
				if (ic == cache.end() || (*ic).second > ns)
				{
					q.push({ np , ns });
					cache[np] = ns;
				}
			}
			// right
			xy = move(p.x_, p.y_, right(p.d_));
			if (valid_move(xy.first, xy.second, md))
			{
				path np{ xy.first, xy.second, right(p.d_), 1 };
				auto ns{ sc + md(xy.second, xy.first) };
				auto ic{ cache.find(np) };
				if (ic == cache.end() || (*ic).second > ns)
				{
					q.push({ np , ns });
					cache[np] = ns;
				}
			}
			// straight
			if (p.steps_ < 3)
			{
				xy = move(p.x_, p.y_, p.d_);
				if (valid_move(xy.first, xy.second, md))
				{
					path np{ xy.first, xy.second, p.d_, ++p.steps_ };
					auto ns{ sc + md(xy.second, xy.first) };
					auto ic{ cache.find(np) };
					if (ic == cache.end() || (*ic).second > ns)
					{
						q.push({ np , ns });
						cache[np] = ns;
					}
				}
			}
		}
	}

	return mn;
}

auto pt2(auto const& in)
{
	auto const& v{ in.first };
	auto s{ in.second };
	stdex::mdspan md(v.data(), v.size() / s, s);
	int xe{ int(md.extent(1)) - 1 };
	int ye{ int(md.extent(0)) - 1 };
	int mn{ std::numeric_limits<int>::max() };
	std::map<path, int> cache;
	std::queue<std::pair<path, int>> q;
	q.push({ { 0, 0, 1, 0 }, 0 }); // East
	q.push({ { 0, 0, 2, 0 }, 0 }); // South
	while (!q.empty())
	{
		auto ps{ q.front() };
		q.pop();
		auto& p{ ps.first };
		auto sc{ ps.second };
		if (p.x_ == xe && p.y_ == ye)
		{
			if (p.steps_ > 3 && sc < mn)
				mn = sc;
		}
		else
		{
			auto ic{ cache.find(p) };
			if (ic != cache.end() && (*ic).second < sc)
				continue;
			if (p.steps_ > 3)
			{
				// left
				auto xy = move(p.x_, p.y_, left(p.d_));
				if (valid_move(xy.first, xy.second, md))
				{
					path np{ xy.first, xy.second, left(p.d_), 1 };
					auto ns{ sc + md(xy.second, xy.first) };
					auto ic{ cache.find(np) };
					if (ic == cache.end() || (*ic).second > ns)
					{
						q.push({ np , ns });
						cache[np] = ns;
					}
				}
				// right
				xy = move(p.x_, p.y_, right(p.d_));
				if (valid_move(xy.first, xy.second, md))
				{
					path np{ xy.first, xy.second, right(p.d_), 1 };
					auto ns{ sc + md(xy.second, xy.first) };
					auto ic{ cache.find(np) };
					if (ic == cache.end() || (*ic).second > ns)
					{
						q.push({ np , ns });
						cache[np] = ns;
					}
				}
			}
			// straight
			if (p.steps_ < 10)
			{
				auto xy = move(p.x_, p.y_, p.d_);
				if (valid_move(xy.first, xy.second, md))
				{
					path np{ xy.first, xy.second, p.d_, ++p.steps_ };
					auto ns{ sc + md(xy.second, xy.first) };
					auto ic{ cache.find(np) };
					if (ic == cache.end() || (*ic).second > ns)
					{
						q.push({ np , ns });
						cache[np] = ns;
					}
				}
			}
		}
	}

	return mn;
}

int main()
{
	auto in{ get_input() };
	std::cout << "pt1 = " << pt1(in) << "\n";
	std::cout << "pt2 = " << pt2(in) << "\n";
}