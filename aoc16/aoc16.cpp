#include <iostream>
#include <string>
#include <vector>
// #include <array>
#include <queue>
#include <algorithm>

//#include "graph.h"

auto get_input()
{
	std::vector<char> v;
	size_t stride { 0 };
	std::string ln;
	while(std::getline(std::cin, ln))
	{
		v.insert(v.end(), ln.begin(), ln.end());
		stride = ln.size();
	}
	return std::make_pair(v, stride);
}

using edge_t = std::pair<int, int>;

edge_t move(edge_t e, int g, int s)
{
	if((e.second == 1 && (e.first + 1 ) % s ) ||
		(e.second == -1 && e.first % s > 0) ||
		(e.second == s && e.first + s < g) ||
		(e.second == -s && e.first >= s))
		return {e.first + e.second, e.second};
	return {-1, -1};
}

auto pt1(auto const& in)
{
	std::vector<char> const& v { in.first };
	size_t s { in.second };
	std::vector<int> visited(v.size(), 0);
	std::queue<edge_t> q;
    q.push({0, 1});
	int n { 0 };
    while (!q.empty())
    {
        auto u = q.front(); q.pop();
		++visited[u.first];
		auto nxt { move(u, v.size(), s)};
		if(nxt.first != -1)
		{
			switch(v[nxt.first])
			{
				case '\\':
					if(nxt.second == 1 || nxt.second == -1)
						q.push({nxt.first, nxt.second * s});
					else
						q.push({nxt.first, nxt.second / s});
					break;
				case '/':
					if(nxt.second == 1 || nxt.second == -1)
						q.push({nxt.first, nxt.second * -s});
					else
						q.push({nxt.first, nxt.second / -s});
					break;
				case '-':
					if(nxt.second == 1 || nxt.second == -1)
						q.push(nxt);
					else
					{
						q.push({nxt.first, 1});
						q.push({nxt.first, -1});
					}
					break;
				case '|':
					if(nxt.second == s || nxt.second == -s)
						q.push(nxt);
					else
					{				
						q.push({nxt.first, s});
						q.push({nxt.first, -s});
					}
					break;
				default:
					q.push(nxt);
					break;
			}
		}
		++n;
		if( n == 10000)
			break;
    }
	n = 0;
	for(auto v : visited)
	{
		std::cout << (v == 0 ? '.' : '*') ;
		++n;
		if( n == s)
		{
			std::cout << "\n";
			n = 0;
		}
	}
	return std::ranges::count_if(visited, [](auto val){ return val > 0;});
}

int main()
{
	auto in{get_input()};
	std::cout << "cell count " << in.first.size() << ", stride = " << in.second << "\n";
	std::cout << "pt1 = " << pt1(in) << "\n";
}