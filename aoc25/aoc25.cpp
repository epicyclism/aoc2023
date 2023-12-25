#include <iostream>
#include <string>
#include <vector>

#include "ctre_inc.h"

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/one_bit_color_map.hpp>
#include <boost/graph/stoer_wagner_min_cut.hpp>
#include <boost/property_map/property_map.hpp>

using graph_t = boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
	boost::no_property, boost::property<boost::edge_weight_t, int>>;

size_t vertex_id_from_name(std::string const& nm)
{
	static std::map<std::string, size_t> nm_vertex;
	auto v = nm_vertex.try_emplace(nm, nm_vertex.size());
	return (*v.first).second;
}

graph_t build_graph()
{
	graph_t rg;
	std::string ln;
	while (std::getline(std::cin, ln))
	{
		auto[m, f, rs] = ctre::match<"([a-z]+): (.*)">(ln);
		for(auto t : ctre::search_all<"([a-z]+)">(rs))
		{
			add_edge(vertex_id_from_name(f.to_string()), vertex_id_from_name(t.to_string()), 1, rg);			
		}
	}
	return rg;
}

auto pt1(auto const& g)
{
	BOOST_AUTO(parities,
       boost::make_one_bit_color_map(
            num_vertices(g), get(boost::vertex_index, g)));
	int w = boost::stoer_wagner_min_cut(
			g, get(boost::edge_weight, g), boost::parity_map(parities));

	int a{0};
	int b{0};
	for (size_t i = 0; i < num_vertices(g); ++i)
    {
        if (get(parities, i))
            ++a;
		else
			++b;
    }
	return a * b;
}

int main()
{
	auto g{build_graph()};
	std::cout << "got " << num_vertices(g) << " things\n";
	std::cout << "pt1 = " << pt1(g) << "\n";
}