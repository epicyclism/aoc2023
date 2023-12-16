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
using graph_t = std::vector<std::vector<edge_t>>;

void add_edge(auto& g, int from, int to, int via )
{
    if (g.size() < from + 1)
        g.resize(from + 1);
    g[from].emplace_back(to, via);
}

bool valid_step_up(size_t s, int f)
{
	return f > s;
}

bool valid_step_down(size_t g, size_t s, int f)
{
	return f + s < g;
}

bool valid_step_left(size_t s, int f)
{
	return f % s > 0;
}

bool valid_step_right(size_t s, int f)
{
	return (f + 1) % s != 0;
}

auto build_g(std::vector<char> const& v, size_t s)
{
	graph_t g;
	for(size_t p{0}; p < v.size(); ++p)
	{
		switch(v[p])
		{
			case '.':
				if( valid_step_up(s, p))
				{
					// going up
					switch(v[p - s])
					{
						case '\\':
							if( valid_step_left(s, p - s))
								add_edge(g, p, p - s - 1, p - s);
							break;
						case '/':
							if(valid_step_right(s, p - s))
								add_edge(g, p, p - s + 1, p - s);
							break;
						case '-':
							if( valid_step_left(s, p - s))
								add_edge(g, p, p - s - 1, p);
							if(valid_step_right(s, p - s))
								add_edge(g, p, p - s + 1, p - s);
							break;
						default:
							add_edge(g, p, p - s, -1);
							break;
					}
				}
				if( valid_step_down(v.size(), s, p))
				{
					// going down
					switch(v[p + s])
					{
						case '\\':
							if(valid_step_right(s, p + s))
								add_edge(g, p, p + s + 1, p + s);
							break;
						case '/':
							if( valid_step_left(s, p + s))
								add_edge(g, p, p + s - 1, p + s);
							break;
						case '-':
							if(valid_step_right(s, p + s))
								add_edge(g, p, p + s + 1, p + s);
							if( valid_step_left(s, p + s))
								add_edge(g, p, p + s - 1, p + s);
							break;
						default:
							add_edge(g, p, p + s, -1);
							break;
					}
				}
				if( valid_step_left(s, p))
				{
					// going left
					switch(v[p - 1])
					{
						case '\\':
							if( valid_step_up(s, p - 1))
								add_edge(g, p, p - s - 1, p - 1);
							break;
						case '/':
							if(valid_step_down(s, v.size(), p - 1))
								add_edge(g, p, p + s - 1, p - 1);
							break;
						case '|':
							if( valid_step_up(s, p - 1))
								add_edge(g, p, p - s - 1, p - 1);
							if(valid_step_down(v.size(), s, p - 1))
								add_edge(g, p, p + s - 1, p - 1);
							break;
						default:
							add_edge(g, p, p - 1, -1);
							break;
					}

				}
				if(valid_step_right(s, p))
				{
					// going right
					switch(v[p + 1])
					{
						case '\\':
							if(valid_step_down(v.size(), s, p + 1))
								add_edge(g, p, p + s + 1, p + 1);
							break;
						case '/':
							if( valid_step_up(s, p + 1))
								add_edge(g, p, p - s + 1, p + 1);
							break;
						case '|':
							if(valid_step_down(v.size(), s, p + 1))
								add_edge(g, p, p + s + 1, p + 1);
							if( valid_step_up(s, p + 1))
								add_edge(g, p, p - s + 1, p + 1);
							break;
						default:
							add_edge(g, p, p + 1, -1);
							break;
					}

				}
				break;
			case '|':
				if(valid_step_up( s, p))
					add_edge(g, p, p - s, -1);
				if(valid_step_down(v.size(), s, p))
					add_edge(g, p, p + s, -1);
				break;
			case '-':
				if( valid_step_left(s, p))
					add_edge(g, p, p - 1, -1);
				if(valid_step_right(s, p))
					add_edge(g, p, p + 1, -1);
				break;
			default:
				break;
		}
	}

	return g;
}

auto pt1(auto const& in)
{
	std::vector<char> const& v { in.first };
	size_t s { in.second };
	auto g { build_g(v, s)};
	for (auto v{ 0 }; v < 10; ++v)
	{
		std::cout << v << " = ";
		for (auto& e : g[v])
			std::cout << "( " << e.first << ", " << e.second << " ) ";
		std::cout << "\n";
	}
	std::vector<int> visited(v.size(), 0);
	std::queue<edge_t> q;
    q.push({0, -1});
	int n { 0 };
    while (!q.empty())
    {
        auto u = q.front(); q.pop();
        for (auto& e : g[u.first])
        {
			++visited[e.first];
			if( e.second != -1)
				++visited[e.second];
			q.push(e);
        }
		++n;
		if( n == 1000000)
			break;
    }

	return std::ranges::count_if(visited, [](auto val){ return val > 0;});
}

int main()
{
	auto in{get_input()};
	std::cout << "cell count " << in.first.size() << ", stride = " << in.second << "\n";
	std::cout << "pt1 = " << pt1(in) << "\n";
}