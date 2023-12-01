#include <iostream>
#include <vector>
#include <string>
#include <numeric>

auto get_input()
{
	std::vector<std::string> vin;
	std::string ln;
	while(std::getline(std::cin, ln))
		vin.emplace_back(ln);
	return vin;
}

int pt1(auto const& in)
{
	return std::accumulate(in.begin(), in.end(), 0, [](auto l, auto& r)
		{
			int ld { r[r.find_first_of("0123456789")] - '0'};
			int rd { r[r.find_last_of("0123456789")] - '0'};
//			std::cout << r << " " << ld << ", " << rd << "\n";
			return l + ld * 10 + rd;
		});
}

int main()
{
	auto in { get_input()};
	std::cout << "got " << in.size() << " input strings.\n";
	std::cout << "pt1 = " << pt1(in) << "\n";
}
