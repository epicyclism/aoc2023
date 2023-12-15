#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <numeric>
#include <array>

#include "ctre_inc.h"

unsigned char HASH(std::string_view s)
{
	unsigned cv = 0;
	for (auto c : s)
	{
		cv += c;
		cv *= 17;
		cv &= 0xff;
	}
	return cv;
}

auto get_input()
{
	std::vector<std::string> vr;
	std::string ln;
	while (std::getline(std::cin, ln))
	{
		for (auto i : ctre::search_all<"[^,]+">(ln))
			vr.push_back(i.str());
	}
	return vr;
}

auto pt1(auto const& in)
{
	return std::accumulate(in.begin(), in.end(), 0, [](auto a, auto& s)
		{
			return a + HASH(s);
		});
}

struct box
{
	std::vector<std::pair<std::string, unsigned char>> lenses_;

	void add_lens(std::string_view label, unsigned char focus)
	{
		auto i = std::find_if(lenses_.begin(), lenses_.end(), [label](auto& l) { return l.first == label; });
		if (i == lenses_.end())
			lenses_.emplace_back(label, focus);
		else
			(*i).second = focus;
	}
	void remove_lens(std::string_view label)
	{
		std::erase_if(lenses_, [label](auto& l) { return l.first == label; });
	}
	int power() const
	{
		int val{ 0 };
		for (int n = 0; n < lenses_.size(); ++n)
			val += (n + 1) * lenses_[n].second;
		return val;
	}
};

auto pt2(auto const& in)
{
	std::array<box, 256> boxes;
	for (auto& i : in)
	{
		auto m = ctre::match<"([^\\-=]+)[\\-=](\\d*)">(i);
		if (m)
		{
			auto h{ HASH( m.get<1>())};
			if (!m.get<2>().view().empty())
				boxes[h].add_lens(m.get<1>(), m.get<2>().to_number<unsigned char>());
			else
				boxes[h].remove_lens(m.get<1>());
		}
	}
	int rv{ 0 };
	for (int n = 0; n < boxes.size(); ++n)
		rv += (n + 1) * boxes[n].power();
	return rv;
}

int main()
{
	auto in{ get_input() };
	std::cout << "pt1 = " << pt1(in) << "\n";
	std::cout << "pt2 = " << pt2(in) << "\n";
}
