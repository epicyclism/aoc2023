#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#define MDSPAN_USE_PAREN_OPERATOR 1
#include <experimental/mdspan>
namespace stdex = std::experimental;

bool mds_equal(auto l, auto r)
{
	if (l.extent(0) == r.extent(0))
	{
		for (auto x{ 0 }; x < l.size(); ++x)
			if (l[x] != r[x])
				return false;
		return true;
	}
	return false;
}

bool check_reflect_horiz(auto md, int n)
{
	auto l{ n - 1 };
	auto r{ n };
	while (l >= 0 && r != md.extent(1))
	{
		auto mdl = stdex::submdspan(md, stdex::full_extent, l);
		auto mdr = stdex::submdspan(md, stdex::full_extent, r);
		if (!mds_equal(mdl, mdr))
			return false;
		--l;
		++r;
	}
	return true;
}

bool check_reflect_vert(auto md, int n)
{
	auto l{ n - 1 };
	auto r{ n };
	while (l >= 0 && r != md.extent(0))
	{
		auto mdl = stdex::submdspan(md, l, stdex::full_extent);
		auto mdr = stdex::submdspan(md, r, stdex::full_extent);
		if (!mds_equal(mdl, mdr))
			return false;
		--l;
		++r;
	}
	return true;
}

int proc_reflect(std::vector<char> const& v, size_t stride, int prev)
{
	stdex::mdspan md(v.data(), v.size() / stride, stride);
	auto mda = stdex::submdspan(md, 0, stdex::full_extent);
	for (int n{ 1 }; n < md.extent(0); ++n)
	{
		auto mdb = stdex::submdspan(md, n, stdex::full_extent);
		if (mds_equal(mda, mdb) && check_reflect_vert(md, n))
		{
			if( prev != n * 100)
				return n * 100;
		}
		mda = mdb;
	}
	auto mda1 = stdex::submdspan(md, stdex::full_extent, 0);
	for (int n{ 1 }; n < md.extent(1); ++n)
	{
		auto mdb = stdex::submdspan(md, stdex::full_extent, n);
		if (mds_equal(mda1, mdb) && check_reflect_horiz(md, n))
		{
			if (prev != n)
				return n;
		}
		mda1 = mdb;
	}
	return 0;
}

auto get_input()
{
	std::vector<std::pair<int, std::vector<char>>> rv;
	std::vector<char> v;
	std::string ln;
	int stride{ 0 };
	while (std::getline(std::cin, ln))
	{
		if (ln.empty())
		{
			rv.emplace_back(stride, std::move(v));
			v.clear();
		}
		else
		{
			v.insert(v.end(), ln.begin(), ln.end());
			stride = ln.size();
		}
	}
	if (!v.empty())
		rv.emplace_back(stride, std::move(v));
	return rv;
}

auto pt12(auto const& in)
{
	int rv1{ 0 };
	int rv2{ 0 };
	for (auto& vs : in)
	{
		auto [s, v] = vs;
		auto rb{ proc_reflect(v, s, -1) };
		rv1 += rb;
		for (auto n{ 0 }; n < v.size(); ++n)
		{
			auto c{ v[n] };
			v[n] = (c == '.') ? '#' : '.';
			if (auto r = proc_reflect(v, s, rb); r )
			{
				rv2 += r;
				break;
			}
			v[n] = c;
		}
	}
	return std::make_pair(rv1, rv2);
}

int main()
{
	auto in{ get_input() };
	auto [pt1, pt2] = pt12(in);
	std::cout << "pt1 = " << pt1 << "\n";
	std::cout << "pt2 = " << pt2 << "\n";
}