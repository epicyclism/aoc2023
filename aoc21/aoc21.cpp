#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <set>
#include <algorithm>
#include <numeric>

#include "graph.h"

struct graph_t
{
	std::vector<char> g_;
	size_t stride_;
	size_t start_;
};

auto get_input()
{
	graph_t g;
	std::string ln;
	while (std::getline(std::cin, ln))
	{
		if (ln.size())
		{
			g.stride_ = ln.size();
			g.g_.insert(g.g_.end(), ln.begin(), ln.end());
		}
	}
	g.start_ = std::distance(g.g_.begin(), std::ranges::find(g.g_, 'S'));
	g.g_[g.start_] = '.';
	return g;
}

void print(graph_t const& g)
{
	std::cout << "\n";
	size_t cnt{ 0 };
	for (auto c : g.g_)
	{
		std::cout << c;
		++cnt;
		if (cnt == g.stride_)
		{
			std::cout << "\n";
			cnt = 0;
		}
	}
}

auto pt1_brute(auto const& in)
{
	std::set<std::pair<size_t, size_t>> cache;
	grid g{ in.g_, in.stride_, [&](auto f, auto t) { return t != '#'; } };
	int tgt{ in.stride_ == 11 ? 6 : 64 };
	std::vector<int> distances(in.g_.size(), -1);
	struct path
	{
		size_t e_;
		size_t d_;
	};
	std::queue<path> q;
	q.push({ in.start_, 0 });
	while (!q.empty())
	{
		auto e{ q.front() };
		q.pop();
		if (e.d_ < tgt)
		{
				for (auto n : g[e.e_])
				{
					if (!cache.contains({ n, e.d_ + 1 }))
					{
						q.push({ n, e.d_ + 1 });
						cache.insert({ n, e.d_ + 1});
					}
				}
		}
		else
			distances[e.e_] = tgt;
	}
	return std::ranges::count(distances, tgt);
}

using int_t = long long;

auto pt12(auto const& in)
{
	grid g{ in.g_, in.stride_, [&](auto f, auto t) { return t != '#'; } };
	int tgt{ in.stride_ == 11 ? 6 : 64 };
	std::vector<int> distances(in.g_.size(), -1);
	std::queue<size_t> q;
	q.push( in.start_);
	distances[in.start_] = 0;
	while (!q.empty())
	{
		auto e{ q.front() };
		q.pop();
		for (auto n : g[e])
		{
			if (distances[n] == -1)
			{
				distances[n] = distances[e] + 1;
				q.push(n);
			}
		}
	}
	auto pt1 = std::accumulate(distances.begin(), distances.end(), 0,
		[=](auto s, auto d) {if (d <= tgt && d % 2 == 0) return s + 1; return s; });

	if (tgt == 6)
		return std::make_pair(pt1, 0LL);

	int_t odd    = std::ranges::count_if(distances, [](auto d) { return d != -1 && d % 2 == 1; });
	int_t even   = std::ranges::count_if(distances, [](auto d) { return d % 2 == 0; });
	int_t odd_c  = std::ranges::count_if(distances, [](auto d) { return d != -1 && d > 65 && d % 2 == 1; });
	int_t even_c = std::ranges::count_if(distances, [](auto d) { return d > 65 && d % 2 == 0; });

	const int_t steps{ 26501365LL };
	const int_t n = (steps - 65) / 131;

	auto pt2 = n * n * even + ((n + 1) * (n + 1)) * odd - (n + 1) * odd_c + n * even_c;

	return std::make_pair(pt1, pt2);
}

int main()
{
	auto in{ get_input() };

	auto [pt1, pt2] = pt12(in);
	std::cout << "pt1 = " << pt1 << "\n";
	std::cout << "pt2 = " << pt2 << "\n";
}
