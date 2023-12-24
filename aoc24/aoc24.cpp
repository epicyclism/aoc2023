#include <iostream>
#include <string>
#include <vector>
#include <optional>
#include <algorithm>

#include "ctre_inc.h"

using int_t = long long;
using pos_t = double;

template<typename T> struct d1
{
	T x_;
	T dx_;
	d1(T x, T dx) : x_{x}, dx_{dx}
	{}
};

template<typename T> struct d3
{
	T x_;
	T y_;
	T z_;
	d3(T x, T y, T z) : x_{x}, y_{y}, z_{z}
	{}
};

template<typename T> struct ln
{
	d3<T> xyz_;
	d3<T> dxyz_;
	ln(T x, T y, T z, T dx, T dy, T dz) :
		xyz_{x, y, z}, dxyz_{ dx, dy, dz}
		{}
};

template<typename T>
auto mc(ln<T> l)
{
	auto m { double(l.dxyz_.y_) / l.dxyz_.x_};
	return std::make_pair(m, l.xyz_.y_ - l.xyz_.x_ * m);  
}

std::optional<d3<double>> intersect(ln<int_t> const& a, ln<int_t> const& b)
{
	auto[m1, c1] = mc(a);
	auto[m2, c2] = mc(b);
	if(m1 == m2)
		return std::nullopt; // parallel
	auto xi {(c2 - c1)/(m1 - m2)};

	return d3{xi, xi * m1 + c1, 0.0};
}

auto get_input()
{
	std::vector<ln<int_t>> v;
	std::string l;
	while(std::getline(std::cin, l))
	{
		if( auto[m, x, y, z, dx, dy, dz] = ctre::match<"(-?\\d+), +(-?\\d+), +(-?\\d+) @ +(-?\\d+), +(-?\\d+), +(-?\\d+)">(l); m)
			v.emplace_back(x.to_number<int_t>(), y.to_number<int_t>(), z.to_number<int_t>(),
				dx.to_number<int_t>(), dy.to_number<int_t>(), dz.to_number<int_t>());
		else
			std::cout << "TILT: " << l << "\n";
	}
	return v;
}

auto pt1(auto const& in)
{
	pos_t xyf {200000000000000}, xyt {400000000000000};
	if(in.size() == 5)
	{
		xyf = 7.0;
		xyt = 27.0; 
	}
	int intersect_cnt{0};
	for(int a { 0 }; a < in.size() - 1; ++a)
	{
		for( int b { a + 1}; b < in.size(); ++b)
		{
			auto ix = intersect(in[a], in[b]);
			if(ix)
			{
				auto val { ix.value()};
				if(((val.x_ - in[a].xyz_.x_) / in[a].dxyz_.x_ > 0) && ((val.x_ - in[b].xyz_.x_) / in[b].dxyz_.x_ > 0))
				{
					if(val.x_ >= xyf && val.x_ <= xyt && val.y_ >= xyf && val.y_ <= xyt)
						++intersect_cnt;
				}
			}
		}
	}
	return intersect_cnt;
}

auto pt2(auto const& in)
{
	std::cout << "\n";
	for(int n { 0}; n < 3; ++n)
	{
		if ( in[n].dxyz_.x_ < 0)
			std::cout << in[n].xyz_.x_ << " - " << -1 * in[n].dxyz_.x_ << " * t" << n << " - x - t" << n << " * X = 0, ";
		else
			std::cout << in[n].xyz_.x_ << " + " << in[n].dxyz_.x_ << " * t" << n << " - x - t" << n << " * X = 0, ";
		if ( in[n].dxyz_.y_ < 0)
			std::cout << in[n].xyz_.y_ << " - " << -1 * in[n].dxyz_.y_ << " * t" << n << " - y - t" << n << " * Y = 0, ";
		else
			std::cout << in[n].xyz_.y_ << " + " << in[n].dxyz_.y_ << " * t" << n << " - y - t" << n << " * Y = 0, ";
		if ( in[n].dxyz_.z_ < 0)
			std::cout << in[n].xyz_.z_ << " - " << -1 * in[n].dxyz_.z_ << " * t" << n << " - z - t" << n << " * Z = 0, ";
		else
			std::cout << in[n].xyz_.z_ << " + " << in[n].dxyz_.z_ << " * t" << n << " - z - t" << n << " * Z = 0, ";
	}
	std::cout << "\n";
	for(int n { 0}; n < 3; ++n)
	{
		if ( in[n].dxyz_.x_ < 0)
			std::cout << in[n].xyz_.x_ << "-" << -1 * in[n].dxyz_.x_ << "*t" << n << "-x-t" << n << "*X=0\n";
		else
			std::cout << in[n].xyz_.x_ << "+" << in[n].dxyz_.x_ << "*t" << n << "-x-t" << n << "*X=0\n";
		if ( in[n].dxyz_.y_ < 0)
			std::cout << in[n].xyz_.y_ << "-" << -1 * in[n].dxyz_.y_ << "*t" << n << "-y-t" << n << "*Y=0\n";
		else
			std::cout << in[n].xyz_.y_ << "+" << in[n].dxyz_.y_ << "*t" << n << "-y-t" << n << "*Y=0\n";
		if ( in[n].dxyz_.z_ < 0)
			std::cout << in[n].xyz_.z_ << "-" << -1 * in[n].dxyz_.z_ << "*t" << n << "-z-t" << n << "*Z=0\n";
		else
			std::cout << in[n].xyz_.z_ << "+" << in[n].dxyz_.z_ << "*t" << n << "-z-t" << n << "*Z=0\n";
	}
	std::cout << "\n";

	return 0;
}

int main()
{
	auto in { get_input()};
	std::cout << "pt1 = " << pt1(in) << "\n";
	std::cout << "pt2 = " << pt2(in) << "\n";
}