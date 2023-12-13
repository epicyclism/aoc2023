#include <iostream>
#include <string>
#include <array>
#include <vector>
#include <algorithm>
#include <numeric>

#include "ctre_inc.h"

struct hand
{
	std::array<char, 5> cards_;
	int bid_;
	int type_;
};

auto get_input()
{
	std::vector<hand> vh;
	std::string ln;
	while (std::getline(std::cin, ln))
	{
		if (auto [m, c, b] = ctre::match<"([\\dAKQJT]+) (\\d+)">(ln); m)
		{
			hand h;
			std::copy(c.view().begin(), c.view().end(), h.cards_.begin());
			h.bid_ = b.to_number<int>();
			vh.push_back(h);
		}
	}
	return vh;
}

char ascii_to_val(char c)
{
	switch (c)
	{
	case '2':
		return 0;
	case '3':
		return 1;
	case '4':
		return 2;
	case '5':
		return 3;
	case '6':
		return 4;
	case '7':
		return 5;
	case '8':
		return 6;
	case '9':
		return 7;
	case 'T':
		return 8;
	case 'J':
		return 9;
	case 'Q':
		return 10;
	case 'K':
		return 11;
	case 'A':
		return 12;
	}
	return 0; // NOT REACHED WITH VALID INPUT...
}

void proc_hand(hand & h)
{
	std::ranges::transform(h.cards_, h.cards_.begin(), [](auto cd) {return ascii_to_val(cd); });
	std::array<char, 16> cnts{ 0 };
	for (auto c : h.cards_)
		++cnts[c];
	std::ranges::sort(cnts, std::greater<>{});

	int t{ 0 };
	switch (cnts[0])
	{
	case 5:
		t = 6; // 5 of a kind
		break;
	case 4:
		t = 5; // 4 of a kind
		break;
	case 3:
		if (cnts[1] == 2)
			t = 4; // full house
		else
			t = 3; // three of a kind
		break;
	case 2:
		if (cnts[1] == 2)
			t = 2; // two pairs
		else
			t = 1; // one pair
		break;
	default:
		t = 0; // all different
		break;
	}
	h.type_ = t;
}

auto pt1(auto in)
{
	for (auto& h : in)
		proc_hand(h);
	std::ranges::sort(in, [](auto& l, auto& r) { if (l.type_ == r.type_) return l.cards_ < r.cards_; return l.type_ < r.type_; });
	int rank{ 0 };
	return std::accumulate(in.begin(), in.end(), 0U, [&rank](auto t, auto& v ) { ++rank; return t + rank * v.bid_; });
}

char ascii_to_val2(char c)
{
	switch (c)
	{
	case 'J':
		return 0;
	case '2':
		return 1;
	case '3':
		return 2;
	case '4':
		return 3;
	case '5':
		return 4;
	case '6':
		return 5;
	case '7':
		return 6;
	case '8':
		return 7;
	case '9':
		return 8;
	case 'T':
		return 9;
	case 'Q':
		return 10;
	case 'K':
		return 11;
	case 'A':
		return 12;
	}
	return 0; // NOT REACHED WITH VALID INPUT...
}

void proc_hand2(hand& h)
{
	std::ranges::transform(h.cards_, h.cards_.begin(), [](auto cd) {return ascii_to_val2(cd); });
	std::array<char, 16> cnts{ 0 };
	char jokers{ 0 };
	for (auto c : h.cards_)
		if (c == 0)
			++jokers;
		else
			++cnts[c];
	std::ranges::sort(cnts, [](auto l, auto r) { return r < l; });
	cnts[0] += jokers;
	int t{ 0 };
	switch (cnts[0])
	{
	case 5:
		t = 6; // 5 of a kind
		break;
	case 4:
		t = 5; // 4 of a kind
		break;
	case 3:
		if (cnts[1] == 2)
			t = 4; // full house
		else
			t = 3; // three of a kind
		break;
	case 2:
		if (cnts[1] == 2)
			t = 2; // two pairs
		else
			t = 1; // one pair
		break;
	default:
		t = 0; // all different
		break;
	}
	h.type_ = t;
}

auto pt2(auto in)
{
	for (auto& h : in)
		proc_hand2(h);
	std::ranges::sort(in, [](auto& l, auto& r) { if (l.type_ == r.type_) return l.cards_ < r.cards_; return l.type_ < r.type_; });
	int rank{ 0 };
	return std::accumulate(in.begin(), in.end(), 0U, [&rank](auto t, auto& v) { ++rank; return t + rank * v.bid_; });
}

int main()
{
	auto in{ get_input() };
	std::cout << "pt1 = " << pt1(in) << "\n";
	std::cout << "pt2 = " << pt2(in) << "\n";
}