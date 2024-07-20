#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <stack>
#include <algorithm>
#include <limits>

#include "graph.h"

struct graph_t
{
	std::vector<char> v_;
	size_t start_;
	size_t finish_;
	size_t stride_;
	graph_t(): start_{0}, finish_{0}, stride_{0}
	{}
};

auto get_input()
{
	graph_t g;
	std::string ln;
	while (std::getline(std::cin, ln))
	{
		g.v_.insert(g.v_.end(), ln.begin(), ln.end());
		if (g.stride_ == 0)
			g.stride_ = ln.size();
	}
	auto& v{ g.v_ };
	g.start_ = std::distance(v.begin(), std::find(v.begin(), v.end(), '.'));
	g.finish_ = v.size() - std::distance(v.rbegin(), std::find(v.rbegin(), v.rend(), '.')) - 1;

	return g;
}

struct edge_t
{
	size_t v_;
	size_t w_;
	edge_t(size_t v, size_t w) : v_{v}, w_{w}
	{}
};

using gg_t = std::map<size_t, std::vector<edge_t>>;

template<typename V> gg_t graph_from_grid(auto const& in, V v)
{
	grid_direct g(in.v_, in.stride_, v);
	gg_t gg;
	struct pth
	{
		size_t v_;
		size_t vf_;
		size_t w_;
	};
	std::vector<bool> visited(g.size(), false);
	std::queue<pth> q;
    q.push({in.start_, in.start_, 0});
    while (!q.empty())
    {
        auto u = q.front(); q.pop();
		auto al { g[u.v_]};
//		std::cout << "( " << al.size() << " }\n";
		if (al.size() == 1 && u.w_ > 0)
		{
			gg[u.vf_].push_back(edge_t(u.v_, al.front().w_));
		}
		else
		if(al.size() < 3)
		{
			for(auto e: al)
			{
				if (!visited[e])
				{
					visited[e] = true;
					++u.w_;
					u.v_ = e;
					q.push(u);
				}
			}
		}
		else
		{
			gg[u.vf_].push_back(edge_t(u.v_, u.w_));
			u.vf_ = u.v_;
			u.w_ = 1;
			for (auto e : al)
			{
				if (!visited[e])
				{
					visited[e] = true;
					u.v_ = e;
					q.push(u);
				}
			}
		}
    }

	return gg;
}

template<typename V> auto ptN(auto const& in, V v)
{
	auto gg { graph_from_grid( in, v)};
	std::cout << gg.size() << "\n";
	for(auto& v: gg[in.start_])
		std::cout << v.v_ << ", " << v.w_ << "\n";

	return 0;
}

int main()
{
	auto in{ get_input() };
	std::cout << "cells = " << in.v_.size() << ", start = " << in.start_ << ", finish = " << in.finish_ << "\n";
	std::cout << "pt1 = " << ptN(in, [&](auto from, auto to)
		{
			if (in.v_[to] == '#')
				return false;
			if (in.v_[from] == '^')
				return to == from - in.stride_;
			if (in.v_[from] == '>')
				return to == from + 1;
			if (in.v_[from] == 'v')
				return to == from + in.stride_;
			if (in.v_[from] == '<')
				return to == from - 1;
			if (in.v_[to] == '^')
				return from == to + in.stride_;
			if (in.v_[to] == '>')
				return from == to - 1;
			if (in.v_[to] == 'v')
				return from == to - in.stride_;
			if (in.v_[to] == '<')
				return from == to + 1;
			return in.v_[from] == '.';
		}) << "\n";
	std::cout << "pt2 = " << ptN(in, [&](auto from, auto to)
		{
			return !(in.v_[from] == '#' || in.v_[to] == '#')   ;
		}) << "\n";
}
