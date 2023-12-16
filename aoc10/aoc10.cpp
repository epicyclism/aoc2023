#include <iostream>
#include <string>
#include <vector>
#include <tuple>
#include <queue>

#define MDSPAN_USE_PAREN_OPERATOR 1
#include <experimental/mdspan>
namespace stdex = std::experimental;

auto get_input()
{
	std::vector<char> g;
	std::string ln;
	size_t S{ 0};
	size_t stride{0};
	while(std::getline(std::cin, ln))
	{
		if(auto p = ln.find('S'); p != std::string::npos)
			S = g.size() + p;
		g.insert(g.end(), ln.begin(), ln.end());
		stride = ln.size();
	}	
	return std::make_tuple(S, stride, g);
}

struct pt
{
	size_t x_;
	size_t y_;
};
bool operator==(pt const& l, pt const& r)
{
	return l.x_ == r.x_ && l.y_ == r.y_;
}

pt N(pt p)
{
	return { p.x_, p.y_ - 1};
}

pt S(pt p)
{
	return { p.x_, p.y_ + 1};
}

pt W(pt p)
{
	return { p.x_ - 1, p.y_};
}

pt E(pt p)
{
	return { p.x_ + 1, p.y_};
}

pt step(auto& md, pt from, pt at)
{
	pt r { at };
//	std::cout << "(" << r.x_ << ", " << r.y_ << ") -> (";
	switch(md(at.y_, at.x_))
	{
		case '|':
			if( at.y_ + 1 == from.y_)
				r.y_ -= 1;
			else
				r.y_ += 1;
			break;
		case '-':
			if( at.x_ + 1 == from.x_)
				r.x_ -= 1;
			else
				r.x_ += 1;
			break;
		case 'L':
			if( at.x_ + 1 == from.x_)
				r.y_ -= 1;
			else
				r.x_ += 1;
			break;
		case 'J':
			if( at.x_ - 1 == from.x_)
				r.y_ -= 1;
			else
				r.x_ -= 1;
			break;
		case '7':
			if(at.x_ - 1 == from.x_)
				r.y_ += 1;
			else
				r.x_ -= 1;
			break;
		case 'F':
			if(at.x_ + 1 == from.x_ )
				r.y_ += 1;
			else
				r.x_ += 1;
			break;
		default:
			std::cout << "step error (" << md(at.y_, at.x_) << "\n";
			exit(-1);
			break;
	}
//	std::cout << r.x_ << ", " << r.y_ << ")\n";

	return r;
}

int follow(auto& md, auto f, auto t)
{
	auto st{f};
	int steps { 1};
//	std::cout << "md(" << st.x_ << ", " << st.y_ << ") == " << md(st.y_, st.x_) << "\n";
//	std::cout << "md(" << t.x_ << ", " << t.y_ << ") == " << md(t.y_, t.x_) << "\n";
	while( t != st)
	{
		auto tmp { t};
		t = step(md, f, t);
		f = tmp;
		++steps;
		md(f.y_, f.x_) |= 0x80;
	}	 
	return steps;
}

auto pt1(auto& g)
{
//	std::cout << "start at " << std::get<0>(g) << ", stride = " << std::get<1>(g) << ", size = " << std::get<2>(g).size() << "\n";
	stdex::mdspan md(std::get<2>(g).data(), std::get<1>(g), std::get<1>(g));
	pt st{std::get<0>(g) % md.stride(0), std::get<0>(g) / md.stride(0)};
//	std::cout << "start at " << st.x_ << ", " << st.y_ << "\n";
	
	// take first exit
	int steps { 1 };
	pt nxt;
	if(nxt = N(st); md(nxt.y_, nxt.x_) == '|' || md(nxt.y_, nxt.x_) == '7' || md(nxt.y_, nxt.x_) == 'F')
		steps = follow(md, st, nxt);
	else
	if(nxt = S(st); md(nxt.y_, nxt.x_) == '|' || md(nxt.y_, nxt.x_) == 'L' || md(nxt.y_, nxt.x_) == 'J')
		steps = follow(md, st, nxt);
	else
	if(nxt = W(st); md(nxt.y_, nxt.x_) == '-' || md(nxt.y_, nxt.x_) == 'L' || md(nxt.y_, nxt.x_) == 'F')
		steps = follow(md, st, nxt);
	else
	if(nxt = E(st); md(nxt.y_, nxt.x_) == '-' || md(nxt.y_, nxt.x_) == 'J' || md(nxt.y_, nxt.x_) == '7')
		steps = follow(md, st, nxt);

	return steps / 2;
}

void print(auto md)
{
	std::cout << "\n";
	for(size_t y{0}; y < md.stride(0); ++y)
	{
		for(size_t x{0}; x < md.stride(0); ++x)
		{
			std::cout << md(y, x);
		}
		std::cout << "\n";
	}
}

constexpr char PIPE [3][3]{' ', '*', ' ', ' ', '*', ' ', ' ', '*', ' '};
constexpr char DASH [3][3]{' ', ' ', ' ', '*', '*', '*', ' ', ' ', ' '};
constexpr char L    [3][3]{' ', '*', ' ', ' ', '*', '*', ' ', ' ', ' '};
constexpr char J    [3][3]{' ', '*', ' ', '*', '*', ' ', ' ', ' ', ' '};
constexpr char SVN  [3][3]{' ', ' ', ' ', '*', '*', ' ', ' ', '*', ' '};
constexpr char F    [3][3]{' ', ' ', ' ', ' ', '*', '*', ' ', '*', ' '};

auto explode(auto md)
{
	std::vector<char> v;
	for(size_t y{0}; y < md.stride(0); ++y)
	{
		for( auto scan {0}; scan < 3; ++scan)
		{
			for(size_t x{0}; x < md.stride(0); ++x)
			{
				switch(md(y, x))
				{
					case ' ':
						v.push_back(' ');
						v.push_back(' ');
						v.push_back(' ');
						break;
					case '|':
						v.push_back(PIPE[scan][0]);
						v.push_back(PIPE[scan][1]);
						v.push_back(PIPE[scan][2]);
						break;
					case '-':
						v.push_back(DASH[scan][0]);
						v.push_back(DASH[scan][1]);
						v.push_back(DASH[scan][2]);
						break;
					case 'L':
						v.push_back(L[scan][0]);
						v.push_back(L[scan][1]);
						v.push_back(L[scan][2]);
						break;
					case 'J':
						v.push_back(J[scan][0]);
						v.push_back(J[scan][1]);
						v.push_back(J[scan][2]);
						break;
					case '7':
						v.push_back(SVN[scan][0]);
						v.push_back(SVN[scan][1]);
						v.push_back(SVN[scan][2]);
						break;
					case 'F':
						v.push_back(F[scan][0]);
						v.push_back(F[scan][1]);
						v.push_back(F[scan][2]);
						break;

				}
			}
		}
	}
	return v;
}

void flood_fill(auto md)
{
	pt p { 0, 0};
	std::queue<pt> q;
	q.push(p);
	while(!q.empty())
	{
		auto n {q.front()};
		q.pop();
		if(md(n.y_, n.x_) == ' ')
		{
			md(n.y_, n.x_) = '#';
			if(n.y_ > 0)
				q.push({n.x_, n.y_ -1});
			if(n.x_ < md.stride(0) - 1)
				q.push({n.x_ + 1, n.y_});
			if(n.y_ < md.stride(0) - 1)
				q.push({n.x_, n.y_ + 1});
			if(n.x_ > 0)
				q.push({n.x_ - 1, n.y_});
		}
	}
}

int count_filled(auto md)
{
	int cnt { 0 };
	for(size_t y{0}; y < md.stride(0); y += 3)
	{
		for(size_t x{0}; x < md.stride(0); x += 3)
		{
			if(md(y, x) == '#' && md(y, x + 1) == '#' && md(y, x+2) == '#' &&
				md(y + 1, x) == '#' && md(y + 1, x + 1) == '#' && md(y + 1, x+2) == '#' &&
				md(y + 2, x) == '#' && md(y + 2, x + 1) == '#' && md(y + 2, x+2) == '#')
				++cnt;
		}
	}
	return cnt;
}

auto pt2(auto& g)
{
	stdex::mdspan md(std::get<2>(g).data(), std::get<2>(g).size() / std::get<1>(g), std::get<1>(g));
	pt st{std::get<0>(g) % md.stride(0), std::get<0>(g) / md.stride(0)};
	for(size_t x{0}; x < md.stride(0); ++x)
	{
		for(size_t y{0}; y < md.stride(0); ++y)
		{
			if( md(y, x) & 0x80)
				md(y, x) &= 0x7f;
			else
				md(y, x) = ' ';
		}
	}
	bool bN{false};
	bool bS{false};
	bool bW{false};
	bool bE{false};
	pt nxt;
	if(nxt = N(st); md(nxt.y_, nxt.x_) == '|' || md(nxt.y_, nxt.x_) == '7' || md(nxt.y_, nxt.x_) == 'F')
		bN = true;
	if(nxt = S(st); md(nxt.y_, nxt.x_) == '|' || md(nxt.y_, nxt.x_) == 'L' || md(nxt.y_, nxt.x_) == 'J')
		bS = true;
	if(nxt = W(st); md(nxt.y_, nxt.x_) == '-' || md(nxt.y_, nxt.x_) == 'L' || md(nxt.y_, nxt.x_) == 'F')
		bW = true;
	if(nxt = E(st); md(nxt.y_, nxt.x_) == '-' || md(nxt.y_, nxt.x_) == 'J' || md(nxt.y_, nxt.x_) == '7')
		bE = true;
	if(bN && bS)
		md(st.y_, st.x_) = '|';
	if(bE && bW)
		md(st.y_, st.x_) = '-';
	if(bN && bE)
		md(st.y_, st.x_) = 'L';
	if(bN && bW)
		md(st.y_, st.x_) = 'J';
	if(bS && bW)
		md(st.y_, st.x_) = '7';
	if(bS && bE)
		md(st.y_, st.x_) = 'F';
//	print(md);
	auto ex{ explode(md)};
	stdex::mdspan md2(ex.data(), md.stride(0) * 3, md.stride(0) * 3);
//	print(md2);
	flood_fill(md2);
//	print(md2);
	auto outside = count_filled(md2);
	auto loop = std::ranges::count(ex, '*') / 3 ;
	auto sz = md.stride(0) * md.stride(0);
//	std::cout << "outside = " << outside << ", loop = " << loop << "size = " << sz << "\n";
	return sz - outside - loop;
}

int main()
{
	auto in {get_input()};

	std::cout << "pt1 = " << pt1(in) << "\n";
	std::cout << "pt2 = " << pt2(in) << "\n";
}