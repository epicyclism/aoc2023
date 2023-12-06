#include <iostream>
#include <string>
#include <vector>

#include "ctre_inc.h"

struct rgb
{
	int r_ = 0;
	int g_ = 0;
	int b_ = 0;
};

auto get_input()
{
	std::vector<std::vector<rgb>> v;
	std::string ln;
	while(std::getline(std::cin, ln))
	{
		v.push_back({});
		ln += ";";
		auto start = ln.find(": ");
		ln.erase(0, start + 2);
		rgb srgb {};
		for(auto m :  ctre::search_all<R"((\d+) (red|green|blue)|(;))">(ln))
		{
			int val { sv_to_t<int>(m.get<1>())};
			if(m.get<2>() == "red")
				srgb.r_ = val;
			if(m.get<2>() == "green")
				srgb.g_ = val;
			if(m.get<2>() == "blue")
				srgb.b_ = val;
			if(m.get<3>())
			{
				v.back().push_back(srgb);
				srgb = {};
			}
		}
	}
	return v;
}


int pt1(auto const& in)
{
	int rv { 0 };
	int gmn { 1 };
	for(auto& gm : in)
	{
		bool possible { true};
		for(auto& rnd: gm)
			if(rnd.r_ > 12 || rnd.g_ > 13 || rnd.b_ > 14)
				possible = false;
		if(possible)
			rv += gmn;
		++gmn;
	}
	return rv;
}

int pt2(auto const& in)
{
	int rv { 0 };
	for(auto& gm: in)
	{
		rgb max {};
		for(auto& rnd: gm)
		{
			if( rnd.r_ > max.r_)
				max.r_ = rnd.r_;
			if(rnd.g_ > max.g_)
				max.g_ = rnd.g_;
			if(rnd.b_ > max.b_)
				max.b_ = rnd.b_;
		}
		rv += max.r_ * max.g_ * max.b_;
	}
	return rv;
}

int main()
{
	auto in { get_input()};
	std::cout << in.size() << " games\n";

	std::cout << "pt1 = " << pt1(in) << "\n";
	std::cout << "pt2 = " << pt2(in) << "\n";
}