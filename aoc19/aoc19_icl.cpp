#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <queue>
#include <algorithm>
#include <numeric>

//#include "boost/icl/discrete_interval.hpp"
#include <boost/icl/closed_interval.hpp>
#include <boost/icl/split_interval_set.hpp>
//#include <boost/icl/interval_set.hpp>

#include "ctre_inc.h"

using namespace boost::icl;

struct edge
{
	closed_interval<int> x_;
	closed_interval<int> m_;
	closed_interval<int> a_;
	closed_interval<int> s_;
	size_t tgt_;
};

edge make_edge(size_t tgt)
{
	return edge { { 1, 4000}, {1, 4000}, {1, 4000}, {1, 4000}, tgt };
}

edge make_edge(char xmas, char op, int val, size_t tgt)
{
	edge e{ { 1, 4000}, {1, 4000}, {1, 4000}, {1, 4000}, tgt };
	closed_interval<int> ii{ 1, 4000 };
	if (op == '<')
		ii = construct<closed_interval<int>>(1, val);
	else
		ii = construct<closed_interval<int>>(val + 1, 4000);
	switch (xmas)
	{
	case 'x':
		e.x_ = ii;
		break;
	case 'm':
		e.m_ = ii;
		break;
	case 'a':
		e.a_ = ii;
		break;
	case 's':
		e.s_ = ii;
		break;
	}
	return e;
}

struct graph_t
{
	std::vector<std::vector<edge>> g_;
	size_t in_;
	size_t A_;
	size_t R_;
};

void print(graph_t const& g)
{
	size_t v{ 0 };
	for (auto& al : g.g_)
	{
		std::cout << v << " : ";
		for (auto& e : al)
			std::cout << e.tgt_ << ", ";
		++v;
		std::cout << "\n";
	}
}

struct part
{
	int x_;
	int m_;
	int a_;
	int s_;
	bool accepted_;
};

size_t id_from_s(std::string_view s)
{
	static std::map<std::string, size_t> id;
	auto ii{ id.find(std::string(s)) };
	if (ii == id.end())
	{
//		std::cout << s << " -> " << id.size() << "\n";
		id.emplace(std::string(s), id.size());
		return id.size() - 1;
	}
//	std::cout << s << " -> " << (*ii).second << "\n";
	return (*ii).second;
}

void add_vertex(auto& g, size_t e)
{
	if (g.size() <= e)
		g.resize(e + 1);
}

auto get_input()
{
	graph_t flows;
	std::vector<part> parts;

	std::string ln;
	while (std::getline(std::cin, ln))
	{
		if (ln.empty())
			break;
		if (auto [m, n, rs] = ctre::match<"([^\\{]+)\\{([^\\}]+)\\}">(ln); m)
		{
			auto vid{ id_from_s(n) };
			add_vertex(flows.g_, vid);
			for (auto [m1, vm, op, val, dest] : ctre::search_all<"([xmas])([><])(\\d+):([^,]+)">(rs))
			{
				auto did{ id_from_s(dest) };
				add_vertex(flows.g_, did);
				flows.g_[vid].emplace_back(make_edge(vm.view()[0], op.view()[0], val.to_number<int>(), did));
			}
			auto [m2, last] = ctre::search<",([a-zA-Z]+)\\}">(ln);
			auto lid{ id_from_s(last) };
			add_vertex(flows.g_, lid);
			flows.g_[vid].emplace_back(make_edge(lid));
		}
		else
			std::cout << "TILT " << ln << "\n";
	}
	flows.in_ = id_from_s("in");
	flows.A_ = id_from_s("A");
	flows.R_ = id_from_s("R");
	while (std::getline(std::cin, ln))
	{
		if (auto [m3, x, m, a, s] = ctre::match<"\\{x=(\\d+),m=(\\d+),a=(\\d+),s=(\\d+)\\}">(ln); m3)
			parts.emplace_back(x.to_number<int>(), m.to_number<int>(), a.to_number<int>(), s.to_number<int>(), false);
		else
			std::cout << "TILT " << ln << "\n";
	}

	return std::make_pair(flows, parts);
}

bool applies(auto& r, auto& p)
{
	return contains(r.x_, p.x_) &&
		contains(r.m_, p.m_) &&
		contains(r.a_, p.a_) &&
		contains(r.s_, p.s_);
}

void apply_flows(auto const& flows, auto& p)
{
	auto id{ flows.in_ };
	while (1)
	{
		for (auto& e : flows.g_[id])
		{
			if (applies(e, p))
			{
				if (e.tgt_ == flows.A_)
				{
					p.accepted_ = true;
					return;
				}
				if (e.tgt_ == flows.R_)
					return;
				id = e.tgt_;
				break;
			}
		}
	}
}

auto pt1(auto const& flows, auto parts)
{
	for (auto& p : parts)
		apply_flows(flows, p);
	return std::accumulate(parts.begin(), parts.end(), 0LL,
		[](auto s, auto& p)
		{
			if (p.accepted_)
				return s + p.x_ + p.m_ + p.a_ + p.s_;
			return s;
		});
}

struct path
{
	size_t v_;
	split_interval_set<closed_interval<int>> x_;
	split_interval_set<closed_interval<int>> m_;
	split_interval_set<closed_interval<int>> a_;
	split_interval_set<closed_interval<int>> s_;
};

auto pt2(auto const& flows)
{
	auto answer{ 0LL };
	std::queue<path> q;
	path pb;
	pb.v_ = flows.in_;
	pb.x_.insert(closed_interval<int>(1, 4000));
	pb.m_.insert(closed_interval<int>(1, 4000));
	pb.a_.insert(closed_interval<int>(1, 4000));
	pb.s_.insert(closed_interval<int>(1, 4000));

	q.push(pb);
	while (!q.empty())
	{
		auto p{ q.front() };
		q.pop();
		if (p.v_ == flows.A_)
		{
			auto ways{ 1LL };
			for (auto& i : p.x_)
				ways *= cardinality(i);
			for (auto& i : p.m_)
				ways *= cardinality(i);
			for (auto& i : p.a_)
				ways *= cardinality(i);
			for (auto& i : p.s_)
				ways *= cardinality(i);
			answer += ways;
			continue;
		}
		if (p.v_ != flows.R_)
		{
			for (auto& e : flows.g_[p.v_])
			{
				path p2;
				p2.v_ = e.tgt_;
#if 0
				add_intersection(p2.x_, p.x_, e.x_);
				add_intersection(p2.m_, p.m_, e.m_);
				add_intersection(p2.a_, p.a_, e.a_);
				add_intersection(p2.s_, p.s_, e.s_);
#else
				p2.s_ = p.s_ - e.s_;
#endif
				q.push(p2);
			}
		}
	}
	return answer;
}

int main()
{
	auto [flows, parts] = get_input();
//	print(flows);
	std::cout << "pt1 = " << pt1(flows, parts) << "\n";
	std::cout << "pt2 = " << pt2(flows) << "\n";
}