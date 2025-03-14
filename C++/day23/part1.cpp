#include <algorithm>
#include <thread>
#include <chrono>
#include <array>
#include <fstream>
#include <iostream>
#include <iterator>
#include <print>
#include <queue>
#include <ranges>
#include <set>
#include <unordered_set>
#include <unordered_map>
#include <vector>


struct ThreeWay
{
	ThreeWay(const std::string_view one, const std::string_view two, const std::string_view three)
		: connections{ {one, two, three} }
	{ 
		std::ranges::sort(connections);
	}

	std::array<std::string_view, 3> connections;

	[[nodiscard]]
	bool operator== (const ThreeWay& other) const noexcept
	{
		return connections == other.connections;
	}
};

struct TrioHash
{
	[[nodiscard]]
	std::size_t operator() (const ThreeWay& t) const noexcept
	{
		std::hash<std::string_view> hash;

		auto h1{ hash(t.connections[0])};
		auto h2{ hash(t.connections[1]) };
		auto h3{ hash(t.connections[2]) };

		return h1 + (h2 << 1) + (h3 << 2);
	}
};

int main()
{
	std::ifstream file("C:/dev/cpp/Advent of Code/day23/input.txt", std::ios::in);

	if (!file.is_open())
	{
		std::println("Failed to open input file.");
		return 1;
	}

	const std::string input(std::istreambuf_iterator<char>{file}, {});

	std::unordered_map<std::string_view, std::unordered_set<std::string_view>> groups;

	auto links{ std::views::split(input, '\n') };

	for (const auto link : links)
	{
		std::string_view pair{ link };
		std::string_view left{ pair.substr(0, 2) };
		std::string_view right{ pair.substr(3) };

		groups[left].insert(right);
		groups[right].insert(left);
	}

	std::unordered_set<ThreeWay, TrioHash> tripplets;

	for (const auto link : links)
	{
		std::string_view pair{ link };
		std::string_view left{ pair.substr(0, 2) };
		std::string_view right{ pair.substr(3) };

		const auto& rightConnections{ groups[right] };

		for (const auto& leftConnection : groups[left])
		{
			if (rightConnections.contains(leftConnection))
			{
				tripplets.insert({ left, right, leftConnection });
			}
		}
	}

	auto tMachines{ std::ranges::count_if(tripplets, [](const ThreeWay& network)
		{
			return std::ranges::any_of(network.connections, [](const std::string_view id)
				{
					return id.starts_with('t');
				});
		}) };

	std::println("{}", tMachines);
}

