#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#include "ctre_inc.h"

using uint_t = unsigned;

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
			tmp.emplace_back(f.to_number<uint_t>(), t.to_number<uint_t>(), s.to_number<uint_t>());
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
	for (auto& mp : txs.txs_)
		std::ranges::sort(mp, [](auto& l, auto& r) { return l.f_ < r.f_; });

	return txs;
}

uint_t apply_map(uint_t val, std::vector<range>const& map)
{
	for (auto& rng : map)
		if (val >= rng.f_ && val < rng.f_ + rng.sz_)
			return rng.t_ + (val - rng.f_);

	return val;
}

uint_t pt1(auto const& in)
{
	uint_t min{ -1U };
	for (auto sd : in.seeds_)
	{
		auto val{ sd };
		for (auto const& mp : in.txs_)
			val = apply_map(val , mp);
		if (val < min)
			min = val;
	}
	return min;
}

uint_t pt2(auto const& in)
{
	uint_t min{ -1U };
	for (uint_t ns{ 0 }; ns != in.seeds_.size(); ns += 2)
	{
		std::cout << "from " << in.seeds_[ns] << " to " << in.seeds_[ns] + in.seeds_[ns + 1];
		for (uint_t sd{ in.seeds_[ns] }; sd < in.seeds_[ns] + in.seeds_[ns + 1]; ++sd)
		{
			auto val{ sd };
			for (auto const& mp : in.txs_)
				val = apply_map(val, mp);
			if (val < min)
				min = val;
		}
		std::cout << " (" << min << ")\n";
	}
	return min;
}

int main()
{
	auto in{ get_input() };
#if 0
	for (auto s : in.seeds_)
		std::cout << s << " ";
	std::cout << "\n";
	std::cout << in.txs_.size() << " maps\n";
	for (auto& m : in.txs_.back())
		std::cout << m.f_ << " " << m.t_ << " " << m.sz_ << "\n";
#endif
	std::cout << "pt1 = " << pt1(in) << "\n";
	std::cout << "pt2 = " << pt2(in) << "\n";
}
