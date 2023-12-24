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

void topological_sort(int v, std::vector<bool>& visited, std::stack<int>& s, auto const& g)
{
	visited[v] = true;
	for(auto e: g[v])
	{
		if(!visited[e])
			topological_sort(e, visited, s, g);
	}
	s.push(v);
}

template<typename V> auto ptN(auto const& in, V v)
{
	grid_direct g(in.v_, in.stride_, v);

	std::vector<int> distance( g.size(), std::numeric_limits<int>::lowest());
	std::vector<bool> visited(g.size(), false);
	std::stack<int> s;
	for(int n{0}; n < g.size(); ++n)
		if(!visited[n])
			topological_sort(n, visited, s, g);
	
	std::vector<int> prev(g.size(), -1);
	distance[in.start_] = 0;
	while(!s.empty())
	{
		auto u { s.top()};
		s.pop();
		if(distance[u] != std::numeric_limits<int>::lowest())
		{
			auto const& vv{ g[u]};
			for(auto n: vv)
			{
				if(distance[n] < distance[u] + 1)
				{
					distance[n] = distance[u] + 1;
					prev[n] = u;
				}
			}
		}
	}

	return distance[in.finish_];
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

// 4914 too low
// 6186 too low