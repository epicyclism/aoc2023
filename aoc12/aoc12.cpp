#include <iostream>
#include <string>
#include <array>
#include <vector>
#include <algorithm>
#include <map>

#include "ctre_inc.h"

using int_t = long long;

struct row
{
	std::string map_;
	std::vector<int> vg_;
};

auto get_input()
{
	std::vector<row> vr;
	std::string ln;
	while (std::getline(std::cin, ln))
	{
		row r;
		auto [m, s, g] = ctre::match<"([\\.\\?#]+) (.*)">(ln);
		r.map_ = s;
		for (auto v : ctre::search_all<"(\\d+)">(g.to_view()))
			r.vg_.push_back(v.to_number<int>());
		vr.emplace_back(r);
	}
	return vr;
}

using graph_t = std::vector<std::array<int, 2>>;

auto make_graph(std::vector<int> const& vg)
{
	graph_t g;
	std::array<int, 2> e;
	e[0] = g.size() + 1; // hash
	e[1] = g.size();     // loop
	g.emplace_back(e);
	for (auto v : vg)
	{
		for (int n{ 0 }; n < v - 1; ++n)
		{
			e[0] = g.size() + 1;
			e[1] = -1;
			g.emplace_back(e);
		}
		e[0] = -1;			// not hash
		e[1] = g.size() + 1;// next
		g.emplace_back(e);
		e[0] = g.size() + 1; // hash
		e[1] = g.size();     // loop
		g.emplace_back(e);
	}
	g.pop_back();
	g.pop_back();
	e[0] = -2;			// not hash
	e[1] = g.size();// loop
	g.emplace_back(e);

	return g;
}

std::map<std::pair<int, char const*>, int_t> cache;

int_t match(graph_t const& g, int v, char const* pc, char const* pce)
{
	if (cache.contains({ v, pc }))
		return cache[{v, pc}];
	if (*pc == 0)
	{
		if (g[v][0] == -2)
		{
//			cache.insert({ {g[v][0], 0}, 1 });
			return 1;
		}
//		cache.insert({ {g[v][0], 0}, 0 });
		return 0;
	}
	int_t rv{ 0 };
	if (*pc == '#')
	{
		if (g[v][0] > 0)
			rv = match(g, g[v][0], pc + 1, pce);
	}
	else
		if (*pc == '.')
		{
			if (g[v][1] != -1)
				rv = match(g, g[v][1], pc + 1, pce);
		}
		else
		{
			if (g[v][0] > 0)
				rv = match(g, g[v][0], pc + 1, pce);
			if (g[v][1] != -1)
				rv += match(g, g[v][1], pc + 1, pce);
		}
	cache.insert({ {v, pc}, rv });
	return rv;
}

auto pt1(auto const& in)
{
	int_t cnt{ 0 };
	for (auto& r : in)
	{
		cache.clear();
		auto g{ make_graph(r.vg_) };
		cnt += match(g, 0, r.map_.c_str(), r.map_.c_str() + r.map_.size());
	}
	return cnt;
}

auto pt2(auto in)
{
	for (auto& r : in)
	{
		std::string sn{ r.map_ };
		std::vector<int> v{ r.vg_ };
		for (int n = 0; n < 4; ++n)
		{
			sn += '?';
			sn.append(r.map_);
			v.insert(v.end(), r.vg_.begin(), r.vg_.end());
		}
		r.map_.swap(sn);
		r.vg_.swap(v);
	}
	int_t cnt{ 0 };
	for (auto& r : in)
	{
		cache.clear();
		auto g{ make_graph(r.vg_) };
		cnt += match(g, 0, r.map_.c_str(), r.map_.c_str() + r.map_.size());
	}
	return cnt;
}

int main()
{
	auto in{ get_input() };

	std::cout << "pt1 = " << pt1(in) << "\n";
	std::cout << "pt2 = " << pt2(in) << "\n";
}