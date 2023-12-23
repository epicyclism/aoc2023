#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <stack>
#include <algorithm>

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

auto pt1(auto const& in)
{
	grid_direct g(in.v_, in.stride_,
		[&](auto from, auto to)
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
		});

	std::stack<std::pair<size_t, std::set<size_t>>> s ;
	s.push({ in.start_, {} });
	size_t longest{ 0 };
	while (!s.empty())
	{
		auto v{ s.top() };
		s.pop();
		if (v.first == in.finish_ && longest < v.second.size())
			longest = v.second.size();
		if (!v.second.contains(v.first))
		{
			v.second.insert(v.first);
			for (auto n : g[v.first])
			{
				s.push({ n, v.second });
			}
		}
	}
	return longest;
}

auto pt2(auto const& in)
{
	grid_direct g(in.v_, in.stride_,
		[&](auto from, auto to)
		{
			if (in.v_[to] == '#')
				return false;
			return true;
		});

	std::stack<std::pair<size_t, std::set<size_t>>> s;
	s.push({ in.start_, {} });
	size_t longest{ 0 };
	while (!s.empty())
	{
		auto v{ s.top() };
		s.pop();
		if (v.first == in.finish_ && longest < v.second.size())
			longest = v.second.size();
		if (!v.second.contains(v.first))
		{
			v.second.insert(v.first);
			for (auto n : g[v.first])
			{
				s.push({ n, v.second });
			}
		}
	}
	return longest;
}

int main()
{
	auto in{ get_input() };
	std::cout << "pt1 = " << pt1(in) << "\n";
	std::cout << "pt2 = " << pt2(in) << "\n";
}