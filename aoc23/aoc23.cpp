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

template<typename V> auto ptN(auto const& in, V v)
{
	grid_direct g(in.v_, in.stride_, v);

	size_t longest{ 0 };
	using pq_t =
		struct
	{
		size_t v_;
		size_t w_;
		std::vector<bool> seen_;
	};
	auto pq_t_cmp = [](auto& l, auto& r) { return l.w_ > r.w_; };
	std::priority_queue<pq_t, std::vector<pq_t>, decltype(pq_t_cmp)> q(pq_t_cmp);
	pq_t pqt;
	pqt.v_ = in.start_;
	pqt.w_ = g.size();
	pqt.seen_.resize(g.size());
	q.push(pqt);
	std::vector<int> distances(g.size(), g.size());
	while (!q.empty())
	{
		auto p = q.top(); q.pop();

		for (auto e : g[p.v_])
		{
			if (!p.seen_[e] && distances[e] > distances[p.v_] - 1 )
			{
				p.seen_[e] = true;
				p.v_ = e;
				p.w_ -= 1;
				distances[e] = distances[p.v_] - 1;
				q.push(p);
			}
		}
	}
	return g.size() - distances[in.finish_];
}

int main()
{
	auto in{ get_input() };
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
			return in.v_[to] != '#';
		}) << "\n";
}

// 4914 too low
// 6186 too low