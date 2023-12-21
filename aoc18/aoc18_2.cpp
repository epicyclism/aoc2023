#include <iostream>
#include <optional>
#include <string>
#include <vector>
#include <queue>
#include <algorithm>
#include <numeric>

#define MDSPAN_USE_PAREN_OPERATOR 1
#include <experimental/mdspan>
namespace stdex = std::experimental;

#include "ctre_inc.h"


using int_t = long long;

struct ln
{
	int_t x_;
	int_t y_;
	int_t xt_;
	int_t yt_;
	auto operator<=>(const ln&) const = default;
};

struct interval
{
	int_t f_;
	int_t t_;
};

auto get_input()
{
	std::vector<ln> v1;
	std::vector<ln> v2;
	std::string ln;
	int x1{ 0 };
	int y1{ 0 };
	int xt1{ 0 };
	int yt1{ 0 };
	int x2{ 0 };
	int y2{ 0 };
	int xt2{ 0 };
	int yt2{ 0 };
	while (std::getline(std::cin, ln))
	{
		if (auto [m, d, c] = ctre::match<"([DLRU]) (\\d+) \\(#[\\da-f]{5}[0123]\\)">(ln); m)
		{
			int cnt{ sv_to_t<int>(c, 16) };
			char dir{ d.view()[0]};
			switch (dir)
			{
			case 'R': //right
				xt1 += cnt;
				break;
			case 'D': // down
				yt1 += cnt;
				break;
			case 'L': // left
				xt1 -= cnt;
				break;
			case 'U': // up
				yt1 -= cnt;
				break;
			default:
				break;
			}
			v1.emplace_back(x1, y1, xt1, yt1);
			x1 = xt1;
			y1 = yt1;
		}
		if (auto [m, c, d] = ctre::match<"[DLRU] \\d+ \\(#([\\da-f]{5})([0123])\\)">(ln); m)
		{
			int cnt{ sv_to_t<int>(c, 16) };
			int dir{ sv_to_t<int>(d)};
			switch (dir)
			{
			case 0: //right
				xt2 += cnt;
				break;
			case 1: // down
				yt2 += cnt;
				break;
			case 2: // left
				xt2 -= cnt;
				break;
			case 3: // up
				yt2 -= cnt;
				break;
			default:
				break;
			}
			v2.emplace_back(x2, y2, xt2, yt2);
			x2 = xt2;
			y2 = yt2;
		}
	}
	return std::make_pair(v1, v2);
}

struct segment_set
{
	int_t base_;
	std::vector<interval> intervals_;
};

auto overlaps(interval l, interval r) -> std::optional<interval>
{
	if (l.t_ <= r.f_ || l.f_ >= r.t_)
		return std::nullopt;
	return interval{ std::max(l.f_, r.f_), std::min(l.t_, r.t_) };
}

void remove(interval& is, interval io)
{
	if (io.f_ > is.f_)
		is.t_ = io.f_;
	else
	if (is.t_ < io.t_)
		is.f_ = io.t_;
}

void remove(std::vector<interval>& v, interval ir)
{
	std::vector<interval> vt;
	for (auto& i : v)
	{
		if (ir.f_ <= i.f_ && ir.t_ >= i.t_)
			continue;
		if (ir.f_ <= i.f_ && ir.t_ > i.f_)
		{
			vt.emplace_back(ir.t_, i.t_);
		}
		else
			if (ir.f_ > i.f_ && ir.t_ >= i.t_)
			{
				vt.emplace_back(i.f_, ir.f_ );
			}
			else
				if (ir.f_ > i.f_ && ir.t_ < i.t_)
				{
					vt.emplace_back(i.f_, ir.f_);
					vt.emplace_back(ir.t_, i.t_);
				}
				else
					vt.emplace_back(i);
	}
	v.swap(vt);
}

int_t calculate_area(auto const& in)
{
	std::vector<ln> v;
	for (auto& l : in)
		if (l.x_ == l.xt_)
			v.emplace_back(std::move(l));
	for(auto& l : v)
		if (l.yt_ < l.y_)
			std::swap(l.yt_, l.y_);
	std::ranges::sort(v, [](auto& l, auto& r) { if (l.x_ == r.x_) return l.y_ < r.y_; return l.x_ < r.x_; });
	std::vector<segment_set> vv(1);
	int_t base_x{ v.front().x_ };
	for (auto& l : v)
	{
		if (l.x_ == base_x)
		{
			vv.back().intervals_.emplace_back(l.y_, l.yt_ + 1);
		}
		else
		{
			base_x = l.x_;
			vv.push_back({});
			vv.back().base_ = base_x;
			vv.back().intervals_.emplace_back(l.y_, l.yt_ + 1);
		}
	}
	for (auto& ll : vv)
	{
		std::cout << ll.base_ << " : ";
		for(auto& l: ll.intervals_)
			std::cout << "( " << l.f_ << " - " << l.t_ << " ) ";
		std::cout << "\n";
	}
	int_t area{ 0 };
	for(auto il { vv.begin()}; il != vv.end() - 1; ++il)
	{
		auto& ints_l{ (*il).intervals_ };
		for (auto ii : ints_l)
		{
			for (auto ir{ il + 1 }; ir != vv.end(); ++ir)
			{
				for (auto& i : (*ir).intervals_)
				{
					if (auto ol{ overlaps(ii, i) }; ol)
					{
						int_t aa{ (std::abs((*ir).base_ - (*il).base_) + 1) * std::abs( (ol.value().t_ - ol.value().f_))};
						std::cout << ol.value().f_ << " <-> " << ol.value().t_ << " : " << (*ir).base_ << ", " << (*il).base_ << ", a + " << aa << "\n";
						area += aa;
						remove(ii , ol.value());
						remove((*ir).intervals_, ol.value());
					}
				}
			}
		}
	}
	return area;
}

auto pt1(auto const& in)
{
	return calculate_area(in);
}

auto pt2(auto const& in)
{
	return calculate_area(in);
}

int main()
{
	auto in{ get_input() };
	std::cout << "pt1 = " << pt1(in.first) << "\n";
//	std::cout << "pt2 = " << pt2(in.second) << "\n";
}
