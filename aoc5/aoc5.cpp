#include <iostream>
#include <string>
#include <vector>

#include "ctre_inc.h"

using uint_t = unsigned;

struct span
{
	uint_t f_;
	uint_t t_;
};

struct range
{
	uint_t f_;
	uint_t t_;
	uint_t sz_;
};

struct tx_set
{
	std::vector<uint_t> seeds_;
	std::vector<std::vector<range>> txs_;
};

auto get_input()
{
	tx_set txs;
	std::string ln;
	std::getline(std::cin, ln);
	for (auto m : ctre::search_all<"\\d+">(ln))
		txs.seeds_.push_back(m.to_number<uint_t>());
	std::vector<range> tmp;
	while (std::getline(std::cin, ln))
	{
		if (auto [m, t, f, s] = ctre::match<"(\\d+) (\\d+) (\\d+)">(ln); m)
		{
			tmp.push_back({f.to_number<uint_t>(), t.to_number<uint_t>(), s.to_number<uint_t>()});
		}
		else
		{
			if (!tmp.empty())
				txs.txs_.emplace_back(std::move(tmp));
			tmp.clear();
		}
	}
	if (!tmp.empty())
		txs.txs_.emplace_back(std::move(tmp));

	return txs;
}

std::vector<span> apply_tx(std::vector<span> in, range const& tx, std::vector<span>& out)
{
	std::vector<span> unhandled;
	for (auto& s : in)
	{
		if (tx.f_ >= s.t_ || (tx.f_ + tx.sz_) <= s.f_)
			unhandled.emplace_back(s);
		else
		{
			if (s.f_ < tx.f_)
			{
				unhandled.push_back({s.f_, tx.f_});
				s.f_ = tx.f_;
			}
			if (s.t_ > tx.f_ + tx.sz_)
			{
				unhandled.push_back({tx.f_ + tx.sz_, s.t_});
				s.t_ = tx.f_ + tx.sz_;
			}
			out.push_back({tx.t_ + s.f_ - tx.f_, tx.t_ + s.t_ - tx.f_});
		}
	}
	return { unhandled };
}

uint_t min_seed_location(auto start, auto const& in)
{
	std::vector<span> txd;
	for (auto& map : in.txs_)
	{
		txd.clear();
		for (auto& tx : map)
			start = apply_tx(start, tx, txd);
		start.insert(start.end(), txd.begin(), txd.end());
	}
	uint_t min{ -1U };
	for (auto& e : start)
	{
		if (e.f_ < min)
			min = e.f_;
	}
	return min;
}

uint_t pt1(auto const& in)
{
	std::vector<span> start;
	for (auto sd : in.seeds_)
		start.push_back({sd, sd + 1});
	return min_seed_location(start, in);
}

uint_t pt2(auto const& in)
{
	std::vector<span> start;
	for (uint_t ns{ 0 }; ns != in.seeds_.size(); ns += 2)
		start.push_back({in.seeds_[ns], in.seeds_[ns] + in.seeds_[ns + 1]});
	return min_seed_location(start, in);
}

int main()
{
	auto in{ get_input() };
	std::cout << "pt1 = " << pt1(in) << "\n";
	std::cout << "pt2 = " << pt2(in) << "\n";
}
