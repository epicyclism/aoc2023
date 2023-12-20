#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <queue>
#include <bitset>
#include <algorithm>
#include <numeric>

#include "ctre_inc.h"

using bs_t = std::bitset<4000>;

struct edge
{
	bs_t x_;
	bs_t m_;
	bs_t a_;
	bs_t s_;
	bs_t xa_;
	bs_t ma_;
	bs_t aa_;
	bs_t sa_;
	size_t tgt_;
};

edge make_edge(size_t tgt)
{
	edge e;
	e.x_.set();
	e.m_.set();
	e.a_.set();
	e.s_.set();
	e.xa_.set();
	e.ma_.set();
	e.aa_.set();
	e.sa_.set();
	e.tgt_ = tgt;
	return e;
}

edge make_edge(char xmas, char op, int val, size_t tgt)
{
	edge e = make_edge(tgt);
	bs_t bs;
	if (op == '<')
		for (size_t n{0 }; n < val - 1; ++n)
			bs.set(n);
	else
		for (size_t n{ size_t(val) }; n < e.x_.size(); ++n)
			bs.set(n);
	switch (xmas)
	{
	case 'x':
		e.x_ = bs;
		bs.flip();
		e.xa_ = bs;
		break;
	case 'm':
		e.m_ = bs;
		bs.flip();
		e.ma_ = bs;
		break;
	case 'a':
		e.a_ = bs;
		bs.flip();
		e.aa_ = bs;
		break;
	case 's':
		e.s_ = bs;
		bs.flip();
		e.sa_ = bs;
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
			std::cout << "( " << e.tgt_ << " - " << e.x_.count() <<
				", " << e.m_.count() << ", " << e.a_.count() << ", " << e.s_.count() << ") ";
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
		id.emplace(std::string(s), id.size());
		return id.size() - 1;
	}
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
	return r.x_.test( p.x_ - 1 ) &&
		r.m_.test(p.m_ - 1) &&
		r.a_.test(p.a_ - 1) &&
		r.s_.test(p.s_ - 1);
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
	bs_t x_;
	bs_t m_;
	bs_t a_;
	bs_t s_;
};

auto pt2(auto const& flows)
{
	auto rej{ 0LL };
	auto answer{ 0LL };
	std::queue<path> q;
	path pb;
	pb.v_ = flows.in_;
	pb.x_.flip();
	pb.m_.flip();
	pb.a_.flip();
	pb.s_.flip();

	q.push(pb);
	while (!q.empty())
	{
		auto p{ q.front() };
		q.pop();
		for (auto& e : flows.g_[p.v_])
		{
			path p2{ p };
			p2.v_ = e.tgt_;
			p2.x_ &= e.x_;
			p2.m_ &= e.m_;
			p2.a_ &= e.a_;
			p2.s_ &= e.s_;
			p.x_ &= e.xa_;
			p.m_ &= e.ma_;
			p.a_ &= e.aa_;
			p.s_ &= e.sa_;
			if (e.tgt_ == flows.A_)
				answer += p2.x_.count() * p2.m_.count() * p2.a_.count() * p2.s_.count();
			else
				if (e.tgt_ != flows.R_)
					q.push(p2);
		}
	}

	return answer;
}

int main()
{
	auto [flows, parts] = get_input();
	std::cout << "pt1 = " << pt1(flows, parts) << "\n";
	std::cout << "pt2 = " << pt2(flows) << "\n";
}