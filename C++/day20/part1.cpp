#include <algorithm>
#include <format>
#include <compare>
#include <fstream>
#include <iostream>
#include <memory>
#include <numeric>
#include <print>
#include <queue>
#include <ranges>
#include <set>
#include <span>
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace mapTile
{
	constexpr char start{ 'S' };
	constexpr char goal { 'E' };
	constexpr char wall { '#' };
	constexpr char empty{ '.' };
}

[[nodiscard]]
constexpr std::array<std::size_t, 4> get_next_moves(const std::size_t index, const std::size_t lineLength)
{
	const auto north{ index - lineLength };
	const auto south{ index + lineLength };
	const auto east{ index + 1 };
	const auto west{ index - 1 };

	return { north, south, east, west };
}

[[nodiscard]]
bool valid_cheat(const std::size_t index, const std::string_view map, const std::size_t lineLength)
{
	if (map[index] != mapTile::wall)
	{
		return false;
	}

	auto [north, south, east, west] { get_next_moves(index, lineLength) };

	if (map[north] == mapTile::empty && map[south] == mapTile::empty)
	{
		return true;
	}

	if (map[east] == mapTile::empty && map[west] == mapTile::empty)
	{
		return true;
	}

	return false;
}

[[nodiscard]]
bool in_bounds(const std::size_t index, const std::string_view map, const std::size_t lineLength)
{
	constexpr std::size_t mapEdgeAndNewline{ 2 };

	auto x{ index % lineLength };
	auto y{ index / lineLength };

	if (x == 0 || x >= lineLength - mapEdgeAndNewline)
	{
		return false;
	}

	if (y == 0 || y >= map.size() / lineLength)
	{
		return false;
	}

	return true;
}

struct Node
{
	std::size_t index{};
	std::size_t tCost{};

	[[nodiscard]]
	bool operator== (const Node& other) const noexcept
	{
		return index == other.index;
	}
};

struct NodeComparer
{
	[[nodiscard]]
	bool operator() (const Node & lhs, const Node& rhs) const noexcept
	{
		return lhs.tCost > rhs.tCost;
	}
};

struct NodeHasher
{
	[[nodiscard]]
	std::size_t operator() (const Node& node) const noexcept
	{
		auto hash{ std::hash<std::size_t>{} };
		auto h1{ hash(node.index) };
		auto h2{ hash(node.tCost) };

		return h1 + (h2 << 1);
	}
};

[[nodiscard]]
std::vector<std::size_t> find_cheats(const std::string_view map, const std::size_t lineLength)
{
	std::vector<std::size_t> cheats;
	std::string clean{ map };

	clean.at(map.find(mapTile::start)) = mapTile::empty;
	clean.at(map.find(mapTile::goal)) = mapTile::empty;

	for (const auto& [i, c] : std::views::enumerate(map))
	{
		if (in_bounds(i, clean, lineLength) && valid_cheat(i, clean, lineLength))
		{
			cheats.push_back(i);
		}
	}

	return cheats;
}


[[nodiscard]]
std::unordered_map<std::size_t, Node> find_path(const std::size_t start, const std::size_t goal, const std::string_view map, const std::size_t lineLength)
{
	std::priority_queue<Node, std::vector<Node>, NodeComparer> openList;
	std::unordered_set<std::size_t> seen;
	std::unordered_map<Node, Node, NodeHasher> path;

	openList.emplace(start, 0);

	while (!openList.empty())
	{
		Node current{ openList.top() };
		openList.pop();

		if (current.index == goal)
		{
			std::unordered_map<std::size_t, Node> pathNodes;

			pathNodes[current.index] = current;

			current = path[current];

			while (current.index != start)
			{
				pathNodes[current.index] = current;
				current = path[current];
			}
			pathNodes[current.index] = current;

			return pathNodes;
		}

		for (const auto& next : get_next_moves(current.index, lineLength))
		{
			if (seen.contains(next) || map[next] == mapTile::wall)
			{
				continue;
			}

			Node node{ next, current.tCost + 1 };
			
			openList.push(node);
			path[node] = current;
		}
		seen.insert(current.index);
	}

	std::println("No path found.");
	return {};
}

std::size_t calculate_time_saved(const std::size_t cheat, const std::unordered_map<std::size_t, Node>& path, const std::size_t lineLength)
{
	const std::size_t steps{ 2 };
	const auto [north, south, east, west] { get_next_moves(cheat, lineLength) };

	std::pair<std::size_t, std::size_t> pathCosts;

	if (path.contains(north) && path.contains(south))
	{
		if (path.at(north).tCost < path.at(south).tCost)
		{
			pathCosts = std::make_pair(path.at(north).tCost, path.at(south).tCost);
		}
		else
		{
			pathCosts = std::make_pair(path.at(south).tCost, path.at(north).tCost);
		}
	}
	else if (path.contains(east) && path.contains(west))
	{
		if (path.at(east).tCost < path.at(west).tCost)
		{
			pathCosts = std::make_pair(path.at(east).tCost, path.at(west).tCost);
		}
		else
		{
			pathCosts = std::make_pair(path.at(west).tCost, path.at(east).tCost);
		}
	}
	else
	{
		std::unreachable();
	}


	return pathCosts.second - (pathCosts.first + steps);
}

int main()
{
	std::ifstream file{ "C:/dev/cpp/Advent of Code/day20/input.txt", std::ios::in };

	if (!file.is_open())
	{
		std::println("Failed to open input file.");
		return 1;
	}

	std::string input{ std::istreambuf_iterator{ file }, {} };

	const std::size_t lineLength{ input.find('\n') + 1 };

	auto path{ find_path(input.find(mapTile::start), input.find(mapTile::goal), input, lineLength) };
	auto possibleCheats{ find_cheats(input, lineLength) };

	std::vector<std::size_t> times;

	for (const auto cheat : possibleCheats)
	{
		times.push_back(calculate_time_saved(cheat, path, lineLength));
	}

	auto cheatsCount{ std::ranges::count_if(times, [](const std::size_t time) { return time >= 100; }) };

	std::println("{}", cheatsCount);
}

