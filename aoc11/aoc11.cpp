#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>

using int64 = long long;
struct pt
{
	int64 x_;
	int64 y_;
	pt(int64 x, int64 y) : x_{x}, y_{y}
	{}
};

pt operator+(pt const& l, pt const& r)
{
	return { l.x_ + r.x_, l.y_ + r.y_ };
}

auto get_input()
{
	std::vector<pt> v;
	std::string ln;
	int64 nln{ 0 };
	while (std::getline(std::cin, ln))
	{
		for (int64 ncl{ 0 }; ncl < ln.size(); ++ncl)
			if (ln[ncl] == '#')
				v.emplace_back(ncl, nln);
		++nln;
	}
	return v;
}

template<typename C, typename V, typename F> void my_adj_diff(C const& c, V& v, F f)
{
	auto i{ c.begin() };
	auto i2{ i + 1 };
	auto e{ c.end() };
	v.push_back(0);
	while (i2 != e)
	{
		v.push_back(f(*i, *i2));
		++i2;
		++i;
	}
}

// replace each blank column and row by amt blanks
void expand(auto& in, int64 amt)
{
	std::ranges::sort(in, [](auto& l, auto& r) { return l.x_ < r.x_; });
	std::vector<int64> tmp;
	my_adj_diff(in, tmp, [](auto& l, auto& r){ return r.x_ - l.x_; });
	std::transform(tmp.begin(), tmp.end(), tmp.begin(), [=](auto v) { if (v < 2) return 0LL; return (v - 1) * (amt - 1); });
	std::partial_sum(tmp.begin(), tmp.end(), tmp.begin());
	std::transform(in.begin(), in.end(), tmp.begin(), in.begin(), [](auto& l, auto r) { return pt{ l.x_ + r, l.y_ }; });

	std::ranges::sort(in, [](auto& l, auto& r) { return l.y_ < r.y_; });
	tmp.clear();
	my_adj_diff(in, tmp, [](auto& l, auto& r) { return r.y_ - l.y_; });
	std::transform(tmp.begin(), tmp.end(), tmp.begin(), [=](auto v) { if (v < 2) return 0LL; return (v - 1) * (amt - 1); });
	std::partial_sum(tmp.begin(), tmp.end(), tmp.begin());
	std::transform(in.begin(), in.end(), tmp.begin(), in.begin(), [](auto& l, auto r) { return pt{ l.x_, l.y_ + r }; });
}

auto sum_distances(auto& in)
{
	int64 sum{ 0 };
	for (auto a{ 0 }; a < in.size(); ++a)
		for (auto b{ a + 1 }; b < in.size(); ++b)
			sum += std::abs(in[a].x_ - in[b].x_) + std::abs(in[a].y_ - in[b].y_);

	return sum;
}

auto pt1(auto in)
{
	expand(in, 2);

	return sum_distances(in);
}

auto pt2(auto in)
{
	expand(in, 1000000);

	return sum_distances(in);
}

int main()
{
	auto in{ get_input() };
	std::cout << "pt1 = " << pt1(in) << "\n";
	std::cout << "pt2 = " << pt2(in) << "\n";
}