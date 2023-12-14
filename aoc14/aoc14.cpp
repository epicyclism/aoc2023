#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#define MDSPAN_USE_PAREN_OPERATOR 1
#include <experimental/mdspan>
namespace stdex = std::experimental;

auto get_input()
{
	std::vector<char> v;
	int stride{ 0 };
	std::string ln;
	while (std::getline(std::cin, ln))
	{
		if (!ln.empty())
		{
			v.insert(v.end(), ln.begin(), ln.end());
			stride = ln.length();
		}
	}
	return std::make_pair(stride, v);
}

int load_n(auto const& v, int s)
{
	stdex::mdspan md(v.data(), v.size() / s, s);
	// each column
	int rv{ 0 };
	for (int c{ 0 }; c < s; ++c)
	{
		auto mdc = stdex::submdspan(md, stdex::full_extent, c);
		int val = mdc.extent(1);
		for (int n{ 0 }; n < mdc.extent(1); ++n)
		{
			if (mdc[n] == 'O')
				rv += val--;
			else
				if (mdc[n] == '#')
					val = mdc.extent(1) - n - 1;
		}
	}

	return rv;
}

auto pt1(auto const& in)
{
	auto [s, v] = in;
	return load_n(v, s);
}

auto tilt_n(auto mdin, auto mdout)
{
	for (int c{ 0 }; c < mdin.extent(0); ++c)
	{
		auto mdcin  = stdex::submdspan(mdin,  stdex::full_extent, c);
		auto mdcout = stdex::submdspan(mdout, stdex::full_extent, c);
		int loc{ 0 };
		for (int n{ 0 }; n < mdcin.extent(1); ++n)
		{
			if (mdcin[n] == 'O')
			{
				mdcout[loc++] = 'O';
			}
			else
				if (mdcin[n] == '#')
				{
					loc = n + 1;
					mdcout[n] = '#';
				}
		}
	}
}

auto tilt_s(auto mdin, auto mdout)
{
	for (int c{ 0 }; c < mdin.extent(0); ++c)
	{
		auto mdcin = stdex::submdspan(mdin, stdex::full_extent, c);
		auto mdcout = stdex::submdspan(mdout, stdex::full_extent, c);
		auto loc{ mdcin.extent(1) - 1 };
		for (int n{ int(mdcin.extent(1)) - 1}; n >= 0; --n)
		{
			if (mdcin[n] == 'O')
			{
				mdcout[loc--] = 'O';
			}
			else
				if (mdcin[n] == '#')
				{
					loc = n - 1;
					mdcout[n] = '#';
				}
		}
	}
}

auto tilt_w(auto mdin, auto mdout)
{
	for (int c{ 0 }; c < mdin.extent(1); ++c)
	{
		auto mdcin = stdex::submdspan(mdin, c, stdex::full_extent);
		auto mdcout = stdex::submdspan(mdout, c, stdex::full_extent);
		int loc{ 0 };
		for (int n{ 0 }; n < mdcin.extent(0); ++n)
		{
			if (mdcin[n] == 'O')
			{
				mdcout[loc++] = 'O';
			}
			else
				if (mdcin[n] == '#')
				{
					loc = n + 1;
					mdcout[n] = '#';
				}
		}
	}
}

auto tilt_e(auto mdin, auto mdout)
{
	for (int c{ 0 }; c < mdin.extent(1); ++c)
	{
		auto mdcin = stdex::submdspan(mdin, c, stdex::full_extent);
		auto mdcout = stdex::submdspan(mdout, c, stdex::full_extent);
		auto loc{ mdcin.extent(0) - 1 };
		for (int n{ int(mdcin.extent(0)) - 1 }; n >= 0; --n)
		{
			if (mdcin[n] == 'O')
			{
				mdcout[loc--] = 'O';
			}
			else
				if (mdcin[n] == '#')
				{
					loc = n - 1;
					mdcout[n] = '#';
				}
		}
	}
}

void cycle(auto& v1, int s)
{
	std::vector<char> v2(v1.size(), '.');

	stdex::mdspan mdin(v1.data(), v1.size() / s, s);
	stdex::mdspan mdout(v2.data(), v2.size() / s, s);
	tilt_n(mdin, mdout);
	std::ranges::fill(v1, '.');
	tilt_w(mdout, mdin);
	std::ranges::fill(v2, '.');
	tilt_s(mdin, mdout);
	std::ranges::fill(v1, '.');
	tilt_e(mdout, mdin);
}

int static_load_n(auto const& v, int s)
{
	stdex::mdspan md(v.data(), v.size() / s, s);
	// each column
	int rv{ 0 };
	for (int c{ 0 }; c < s; ++c)
	{
		auto mdc = stdex::submdspan(md, stdex::full_extent, c);
		int val = mdc.extent(1);
		for (int n{ 0 }; n < mdc.extent(1); ++n)
		{
			if (mdc[n] == 'O')
				rv += mdc.extent(1) - n;
		}
	}

	return rv;
}

int pt2(auto const& in)
{
	auto [s, v] = in;
	std::vector<int> vr;
	for (int cnt{ 0 }; cnt < 512; ++cnt)
	{
		cycle(v, s);
		vr.push_back(static_load_n(v, s));
	}
	// find the cycle
	auto b{ std::find(vr.begin(), vr.end(), vr.back()) };
	auto n{ std::find(b + 1, vr.end(), vr.back()) };
	if (n == vr.end())
	{
		std::cout << "Expansion too small to find cycle, adjust and rebuild!\n";
		return -1;
	}
	// assuming cycle length + preamble less than 512....
	auto cycle_len{ std::distance(b, n) };
	auto preamble{ std::distance(vr.begin(), b) };
	auto val_at{ vr.begin() + (1000000000 - preamble) % cycle_len + preamble - 1 };
	return *val_at;
}

int main()
{
	auto in{ get_input() };
	std::cout << "pt1 = " << pt1(in) << "\n";
	std::cout << "pt2 = " << pt2(in) << "\n";
}