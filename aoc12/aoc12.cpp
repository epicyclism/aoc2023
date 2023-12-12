#include <iostream>
#include <string>
#include <array>
#include <vector>
#include <algorithm>

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
		{
			r.vg_.push_back(v.to_number<int>());
		}
		vr.emplace_back(r);
	}
	return vr;
}

struct edge
{
	char c_ = 0;
	char nxt_ = -1;
};

using graph_t = std::vector<std::array<edge, 2>>;

auto make_graph(std::string const& s)
{
	graph_t g;
	for (auto c : s)
	{
		std::array<edge, 2> e;
		switch (c)
		{
		case '.':
			e[0].c_ = '.';
			e[0].nxt_ = g.size() + 1;
			e[1].c_ = ' ';
			e[1].nxt_ = -1;
			break;
		case '#':
			e[0].c_ = '#';
			e[0].nxt_ = g.size() + 1;
			e[1].c_ = ' ';
			e[1].nxt_ = -1;
			break;
		case '?':
			e[0].c_ = '#';
			e[0].nxt_ = g.size() + 1;
			e[1].c_ = '.';
			e[1].nxt_ = g.size() + 1;
			break;
		default:
			std::cout << "TILT\n";
			break;
		}
		g.emplace_back(std::move(e));
	}
	g.emplace_back();
	return g;
}

std::vector<int> to_g(std::string const& s)
{
	std::vector<int> rv;
	int cnt{ 0 };
	bool inhash{ false };
	for (auto c : s)
	{
		if (c == '#')
		{
			++cnt;
			inhash = true;
		}
		else
		{
			if (inhash)
			{
				rv.push_back(cnt);
				cnt = 0;
				inhash = false;
			}
		}
	}
	if (cnt)
		rv.push_back(cnt);
	return rv;
}

bool special_eq(std::vector<int> const& l, std::vector<int> const& r)
{
	if (l.empty())
		return true;
	if (l.size() > r.size())
		return false;
	size_t n{ 0 };
	for (; n < l.size() - 1; ++n)
	{
		if (l[n] != r[n])
			return false;
	}
	return l[n] <= r[n];
}

//void enumerate(graph_t const& g, std::string s, char v, std::vector<std::string>& vs, std::vector<int> const& vg, int_T& cnt)
void enumerate(graph_t const& g, std::string s, char v, std::vector<int> const& vg, int_t& cnt)
{
	if (!special_eq(to_g(s), vg))
		return;
	if (g[v][0].nxt_ == -1)
	{
//		std::cout << s << "\n";
//		vs.emplace_back(s);
		if (to_g(s) == vg)
			++cnt;
		return;
	}
	s += g[v][0].c_;
	enumerate(g, s, g[v][0].nxt_, vg, cnt);
	if (g[v][1].nxt_ != -1)
	{
		s.back() = g[v][1].c_;
		enumerate(g, s, g[v][1].nxt_, vg, cnt);
	}
}

//void enumerate(graph_t const& g, std::vector<std::string>& vs, std::vector<int> const& vg)
void enumerate(graph_t const& g, std::vector<int> const& vg, int_t& cnt)
{
	char v{ 0 };
	std::string s;
	enumerate(g, s, v, vg, cnt);
}

int proc_vs(std::vector<int> const& vg, std::vector<std::string> const& vs)
{
	int c{ 0 };
	for (auto& s: vs)
	{
		c += std::ranges::equal(vg, to_g(s));
	}
	return c;
}

auto pt1(auto const& in)
{
#if 0
	for (auto& r : in)
	{
		std::cout << r.map_ << " [ ";
		for (auto i : r.vg_)
			std::cout << i << " ";
		std::cout << " ]\n";
	}
#endif
	int_t cnt{ 0 };
	std::vector<std::string> vs;
	for (auto& r : in)
	{
		vs.clear();
		enumerate(make_graph(r.map_), r.vg_, cnt);
//		cnt += proc_vs(r.vg_, vs);
//		std::cout << "\n";
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
			v.append_range(r.vg_);
		}
		r.map_.swap(sn);
		r.vg_.swap(v);
	}
	int_t cnt{ 0 };
	std::cout << "0\n";
//	std::vector<std::string> vs;
	for (auto& r : in)
	{
//		vs.clear();
		enumerate(make_graph(r.map_), r.vg_, cnt);
//		cnt += proc_vs(r.vg_, vs);
		std::cout << cnt << "\n";
	}
	return cnt;
}

int main()
{
	auto in{ get_input() };
	std::cout << "got " << in.size() << " rows\n";
	std::cout << "pt1 = " << pt1(in) << "\n";
	std::cout << "pt2 = " << pt2(in) << "\n";
}