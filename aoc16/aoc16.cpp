#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <algorithm>

auto get_input()
{
	std::vector<char> v;
	int stride { 0 };
	std::string ln;
	while(std::getline(std::cin, ln))
	{
		v.insert(v.end(), ln.begin(), ln.end());
		stride = ln.size();
	}
	return std::make_pair(v, stride);
}

using edge_t = std::pair<int, int>;

edge_t move(edge_t e, int g, unsigned s)
{
	if((e.second == 1 && (e.first + 1 ) % s ) ||
		(e.second == -1 && e.first % s > 0) ||
		(e.second == s && e.first + s < g) ||
		(e.second == -s && e.first >= s))
		return {e.first + e.second, e.second};
	return {-1, -1};
}

int dir_to_bit(int d)
{
	if( d == 1)
		return 1;
	if( d == -1)
		return 2;
	if( d < 0)
		return 4;
	return 8;
}

int compute_from(auto const& in, int start, int dir)
{
	std::vector<char> const& v { in.first };
	int s { in.second };
	std::vector<int> visited(v.size(), 0);
	std::queue<edge_t> q;
    q.push({start, dir});
    while (!q.empty())
    {
        auto u = q.front(); q.pop();
		if(u.first != -1 && !(visited[u.first] & dir_to_bit(u.second)))
		{
			visited[u.first] |= dir_to_bit(u.second);
			switch(v[u.first])
			{
				case '\\':
					if(u.second == 1 || u.second == -1)
						q.push(move({u.first, u.second * s}, v.size(), s));
					else
						q.push(move({u.first, u.second / s}, v.size(), s));
					break;
				case '/':
					if(u.second == 1 || u.second == -1)
						q.push(move({u.first, u.second * -s}, v.size(), s));
					else
						q.push(move({u.first, u.second / -s}, v.size(), s));
					break;
				case '-':
					if(u.second == 1 || u.second == -1)
						q.push(move(u, v.size(), s));
					else
					{
						q.push(move({u.first, 1}, v.size(), s));
						q.push(move({u.first, -1}, v.size(), s));
					}
					break;
				case '|':
					if(u.second == s || u.second == -s)
						q.push(move(u, v.size(), s));
					else
					{				
						q.push(move({u.first, s}, v.size(), s));
						q.push(move({u.first, -s}, v.size(), s));
					}
					break;
				default:
					q.push(move(u, v.size(), s));
					break;
			}
		}
    }

	return std::ranges::count_if(visited, [](auto val){ return val > 0;});
}

auto pt1(auto const& in)
{
	return compute_from(in, 0, 1);
}

auto pt2(auto const& in)
{
	std::vector<char> const& v { in.first };
	int s { in.second };

	int mx { 0 };
	for(int st = 0; st < s; ++st)
	{
		int v { compute_from(in, st, s)};
		if( v > mx)
			mx = v;
	}
	for(int st = 0; st < v.size(); st += s)
	{
		int v { compute_from(in, st, 1)};
		if( v > mx)
			mx = v;
	}
	for(int st = v.size() - s; st < v.size(); ++st)
	{
		int v { compute_from(in, st, -s)};
		if( v > mx)
			mx = v;
	}
	for(int st = s - 1; st < v.size(); st += s)
	{
		int v { compute_from(in, st, -1)};
		if( v > mx)
			mx = v;
	}
	return mx;
}

int main()
{
	auto in{get_input()};

	std::cout << "pt1 = " << pt1(in) << "\n";
	std::cout << "pt2 = " << pt2(in) << "\n";
}