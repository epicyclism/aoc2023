#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <queue>

#include "ctre_inc.h"

struct gate
{
	std::map<std::string, char> min_;
	char type_; // % flip flop, & conjoiner, b broadcaster
	int  cnt_;
	char  state_;
	std::vector<std::string> outs_;
};

using graph_t = std::map<std::string, gate>;

auto get_input()
{
	graph_t g;
	std::string ln;
	while (std::getline(std::cin, ln))
	{
		auto [m, t, n, rs] = ctre::match<"([%&b])([^ ]+) -> (.+)">(ln);
		auto [ii,b] = g.emplace(n.to_string(), gate());
		(*ii).second.type_ = (t ? t.view()[0] : 'b');
		(*ii).second.state_ = 0;
		for (auto [m, v] : ctre::search_all<"([a-z]+),?">(rs))
			(*ii).second.outs_.emplace_back(v.to_string());
	}
	for (auto& gt : g)
	{
		for (auto& s : gt.second.outs_)
			g[s].min_[gt.first] = 0;
	}

	return g;
}

void print(graph_t const& g)
{
	for (auto& gt : g)
	{
		std::cout << gt.first << " : ";
		std::cout << gt.second.type_ << " (";
		std::cout << gt.second.cnt_ << ") -> ";
		for (auto& to : gt.second.outs_)
			std::cout << to << " ";
		std::cout << "\n";
	}
}

struct pulse
{
	int val_;
	std::string from_;
	std::string dest_;
};

auto pt1(auto in)
{
	int zero{ 0 };
	int one{ 0 };
	std::queue<pulse> q;
	pulse ps{ 0, "button", "roadcaster" };
	for (int n = 0; n < 1000; ++n)
	{
		q.push(ps);
		while (!q.empty())
		{
			auto p{ q.front() };
			q.pop();
//			std::cout << p.from_ << " -> " << p.val_ << " " << p.dest_ << "\n";
			if (p.val_)
				++one;
			else
				++zero;
			auto& g{ in[p.dest_] };
			switch (g.type_)
			{
			case 'b':
				for (auto& d : g.outs_)
					q.push({ p.val_, p.dest_, d });
				break;
			case '%':
				if (p.val_ == 0) // low
				{
					g.state_ = !g.state_;
					for (auto& d : g.outs_)
						q.push({ g.state_, p.dest_, d });
				}
				break;
			case '&':
				g.min_[p.from_] = p.val_;
				{
					bool allhigh{ true };
					for (auto& m : g.min_)
						if (!m.second)
							allhigh = false;
					for (auto& d : g.outs_)
						q.push({ !allhigh, p.dest_, d });
				}
				break;
			}
		}
	}
	return one * zero;
}

auto pt2(auto in)
{
	std::queue<pulse> q;
	pulse ps{ 0, "button", "roadcaster" };
	int n = 1;
	while(1)
	{
		if (n % 1000000 == 0)
			std::cout << n << "\n";
		q.push(ps);
		while (!q.empty())
		{
			auto p{ q.front() };
			q.pop();
			//			std::cout << p.from_ << " -> " << p.val_ << " " << p.dest_ << "\n";
			if (p.dest_ == "rx" && p.val_ == 0)
				return n;
			auto& g{ in[p.dest_] };
			switch (g.type_)
			{
			case 'b':
				for (auto& d : g.outs_)
					q.push({ p.val_, p.dest_, d });
				break;
			case '%':
				if (p.val_ == 0) // low
				{
					g.state_ = !g.state_;
					for (auto& d : g.outs_)
						q.push({ g.state_, p.dest_, d });
				}
				break;
			case '&':
				g.min_[p.from_] = p.val_;
				{
					bool allhigh{ true };
					for (auto& m : g.min_)
						if (!m.second)
							allhigh = false;
					for (auto& d : g.outs_)
						q.push({ !allhigh, p.dest_, d });
				}
				break;
			}
		}
		++n;
	}
	return -1;
}

int main()
{
	auto in{ get_input() };
//	print(in);
	std::cout << "pt1 = " << pt1(in) << "\n";
	std::cout << "pt2 = " << pt2(in) << "\n";
}