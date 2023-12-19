#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <numeric>

#include "ctre_inc.h"

enum condition { x, m, a, s, u };
struct rule
{
	condition c_;
	char      op_;
	int       val_;
	std::string tgt_;
};

condition condition_from_char(char c)
{
	switch (c)
	{
	case 'x':
		return x;
	case 'm':
		return m;
	case 'a':
		return a;
	case 's':
		return s;
	}
	return u;
}


struct part
{
	int x_;
	int m_;
	int a_;
	int s_;
	bool accepted_;
};

auto get_input()
{
	std::map<std::string, std::vector<rule>> flows;
	std::vector<part> parts;

	std::string ln;
	while (std::getline(std::cin, ln))
	{
		if (ln.empty())
			break;
		if (auto [m, n, rs] = ctre::match<"([^\\{]+)\\{([^\\}]+)\\}">(ln); m)
		{
			auto& i = flows[n.to_string()];
			for (auto [m1, vm, op, val, dest] : ctre::search_all<"([xmas])([><])(\\d+):([^,]+)">(rs))
				i.emplace_back( condition_from_char(vm.view()[0]), op.view()[0], val.to_number<int>(), dest.to_string()) ;
			auto [m2, last] = ctre::search<",([a-zA-Z]+)\\}">(ln);
			i.emplace_back(u, 0, 0, last.to_string());
		}
		else
			std::cout << "TILT " << ln << "\n";
	}
	while (std::getline(std::cin, ln))
	{
		if (auto [m3, x, m, a, s] = ctre::match<"\\{x=(\\d+),m=(\\d+),a=(\\d+),s=(\\d+)\\}">(ln); m3)
		{
			parts.emplace_back(x.to_number<int>(), m.to_number<int>(), a.to_number<int>(), s.to_number<int>(), false);
		}
		else
			std::cout << "TILT " << ln << "\n";
	}

	return std::make_pair(flows, parts);
}

bool applies(auto& r, auto& p)
{
	switch (r.c_)
	{
	case x:
		if (r.op_ == '<' && p.x_ < r.val_)
			return true;
		if (r.op_ == '>' && p.x_ > r.val_)
			return true;
		break;
	case m:
		if (r.op_ == '<' && p.m_ < r.val_)
			return true;
		if (r.op_ == '>' && p.m_ > r.val_)
			return true;
		break;
	case a:
		if (r.op_ == '<' && p.a_ < r.val_)
			return true;
		if (r.op_ == '>' && p.a_ > r.val_)
			return true;
		break;
	case s:
		if (r.op_ == '<' && p.s_ < r.val_)
			return true;
		if (r.op_ == '>' && p.s_ > r.val_)
			return true;
		break;

	}
	return false;
}

void apply_flows(auto const& flows, auto& p)
{
	auto flow = flows.find("in");
	while (1)
	{
		for (auto& r : (*flow).second)
		{
			if (r.c_ == u)
			{
				if (r.tgt_ == "A")
				{
					p.accepted_ = true;
					return;
				}
				if (r.tgt_ == "R")
					return;
				flow = flows.find(r.tgt_);
				break;
			}
			if (applies(r, p))
			{
				if (r.tgt_ == "A")
				{
					p.accepted_ = true;
					return;
				}
				if (r.tgt_ == "R")
					return;
				flow = flows.find(r.tgt_);
				break;
			}
		}
	}
}

auto pt1(auto const& flows, auto parts)
{
	for (auto& p : parts)
	{
		apply_flows(flows, p);
	}
	return std::accumulate(parts.begin(), parts.end(), 0LL,
		[](auto s, auto& p)
		{
			if (p.accepted_)
				return s + p.x_ + p.m_ + p.a_ + p.s_;
			return s;
		});
}

int main()
{
	auto [flows, parts] = get_input();
	std::cout << "got " << flows.size() << " flows.\n";
	std::cout << "got " << parts.size() << " parts.\n";
	std::cout << "pt1 = " << pt1(flows, parts) << "\n";
}