#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>

#include "ctre_inc.h"

auto get_input()
{
	std::vector<int> v;
	std::vector<int> win;
	std::vector<int> have;
	std::vector<int> tv;
	std::string ln;
	while (std::getline(std::cin, ln))
	{
		win.clear();
		have.clear();
		bool b{ false };
		auto be{ ln.find(": ") };
		for (auto m : ctre::range<R"((\d+)|(\|))">(ln.begin() + be, ln.end()))
		{
			if (m.get<2>())
				b = true;
			else
			if (b)
				have.push_back(sv_to_t<int>(m.get<1>()));
			else
				win.push_back(sv_to_t<int>(m.get<1>()));
		}
#if 1
		std::ranges::sort(win);
		std::ranges::sort(have);
		tv.clear();
		std::ranges::set_intersection(win, have, std::back_inserter(tv));
		v.emplace_back(static_cast<int>(tv.size()));
#else
		int wins{0};
		for(auto w : win)
			for(auto g : have)
				if( w == g)
					++wins;
		v.push_back(wins);
#endif
	}
	return v;
}

constexpr int scores[]{ 0, 1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, };

int pt1(auto const& in)
{
	int rv{ 0 };
	for (auto& c : in)
		rv += scores[c];

	return rv;
}

auto pt2(auto const& in)
{
	std::vector<unsigned> cnts(in.size(), 1);
	for (int card{ 0 }; card < in.size(); ++card)
	{
		for (int b{ card + 1 }; b < card + 1 + in[card]; ++b)
			cnts[b] += cnts[card];
	}
	return std::accumulate(cnts.begin(), cnts.end(), 0);
}

int main()
{
	auto in{ get_input() };
	std::cout << "pt1 = " << pt1(in) << "\n";
	std::cout << "pt2 = " << pt2(in) << "\n";
}
