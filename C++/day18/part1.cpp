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
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <vector>


namespace mapData
{
	// Example
	//constexpr std::size_t size{ 7 * 7 };
	//constexpr std::size_t lineLength{ 7 };
	//constexpr int linesToRead{ 12 };

	// Puzzle Input
	constexpr std::size_t size{ 71 * 71 };
	constexpr std::size_t lineLength{ 71 };
	constexpr int linesToRead{ 1024 };

	constexpr std::size_t start{ 0 };
	constexpr std::size_t goal { mapData::size - 1 };
}

namespace mapTile
{
	constexpr char empty{ '.' };
	constexpr char corrupted{ '#' };
}

std::size_t coordinate_to_index(const int x, const int y)
{
	return { y * mapData::lineLength + x };
}

void printMap(const std::string_view map)
{
	for (const auto& [i, c] : std::views::enumerate(map))
	{
		if (i > 0 && i % mapData::lineLength == 0)
		{
			std::print("\n{}", c);
		}
		else
		{
			std::print("{}", c);
		}
	}
	std::println();
}

enum class Direction
{
	north,
	south,
	east,
	west,
};

struct Node
{
	std::size_t index{};
	Direction direction{};
	std::size_t tCost{};
	std::size_t hCost{};

	auto operator<=> (const Node& other) const = default;
};

struct NodeComparer
{
	bool operator() (const Node& lhs, const Node& rhs) const noexcept
	{
		return lhs.hCost > rhs.hCost;
	}
};

struct NodeHasher
{
	std::size_t operator() (const Node& node) const noexcept
	{
		auto h1{ std::hash<std::size_t>{}( node.index ) };
		auto h2{ std::hash<std::size_t>{}( node.tCost ) };
		
		return h1 ^ (h2 << 1);
	}
};

std::size_t manhattan_distance(const std::size_t index)
{
	auto cx{static_cast<long long>( index % mapData::lineLength) };
	auto cy{static_cast<long long>( index / mapData::lineLength) };
	auto gx{static_cast<long long>( mapData::goal % mapData::lineLength) };
	auto gy{static_cast<long long>( mapData::goal / mapData::lineLength) };

	return std::abs(cx - gx) + std::abs(cy - gy);
}

std::size_t get_next_index(const std::size_t previous, const int direction)
{
	std::size_t index{ 0 };

	switch (static_cast<Direction>(direction))
	{
	case Direction::north:
	{
		if (previous < mapData::lineLength)
		{
			return index;
		}

		index = previous - mapData::lineLength;
		break;
	}

	case Direction::south:
	{
		if (previous > mapData::size - mapData::lineLength - 1)
		{
			return index;
		}

		index = previous + mapData::lineLength;
		break;
	}

	case Direction::east:
	{
		if (previous % mapData::lineLength == mapData::lineLength - 1)
		{
			return index;
		}

		index = previous + 1;
		break;
	}

	case Direction::west:
	{
		if (previous % mapData::lineLength == 0)
		{
			return index;
		}

		index = previous - 1;
		break;
	}

	default:
		throw std::invalid_argument("Impossible direction: " + std::to_string(direction));
	}

	return index;
}

short travel_cost(const Direction from, const int to)
{
	return static_cast<Direction>(to) == from ? 1 : 2;
}

struct Result
{
	std::unordered_set<std::size_t> path;
	std::size_t steps;
};

Result findPath(const std::string_view map)
{
	std::priority_queue<Node, std::vector<Node>, NodeComparer> openList;
	std::unordered_set<std::size_t> closedList{ 0 };
	std::unordered_map<Node, Node, NodeHasher> pathNodes{ 0 };

	openList.emplace(mapData::start, Direction::east, 0, 0);

	while (!openList.empty())
	{
		Node current{ openList.top() };
		openList.pop();

		if (mapData::goal == current.index)
		{
			Result result{ {}, current.tCost };

			while (current.index != mapData::start)
			{
				result.path.insert(current.index);
				current = pathNodes[current];
			}

			return result;
		}

		for (const auto next : std::views::iota(0, 4))
		{
			auto nextIndex{ get_next_index(current.index, next) };

			if (closedList.contains(nextIndex) || map.at(nextIndex) == mapTile::corrupted)
			{
				continue;
			}

			auto tCost{ current.tCost + 1 };
			auto hCost{ manhattan_distance(nextIndex) + tCost };

			Node node{ nextIndex, static_cast<Direction>(next), tCost, hCost };

			openList.push(node);
			pathNodes[node] = current;
		}
		closedList.insert(current.index);
	}

	return {};
}

int main()
{
	std::ifstream file{ "C:/dev/cpp/Advent of Code/day18/input.txt", std::ios::in };

	if (!file.is_open())
	{
		std::println("Failed to open input file.");
		return 1;
	}

	std::string input;

	std::string map(mapData::size, mapTile::empty);
	map.at(0) = 'S';

	int x{};
	int y{};

	for (const auto _ : std::views::iota(0, mapData::linesToRead))
	{
		std::getline(file, input);
		std::string_view coordinates{ input };
		auto comma{ coordinates.find(',') };

		auto textX{ coordinates.substr(0, comma) };
		auto textY{ coordinates.substr(comma + 1) };

		std::from_chars(textX.data(), textX.data() + textX.size(), x);
		std::from_chars(textY.data(), textY.data() + textY.size(), y);

		map.at(coordinate_to_index(x, y)) = mapTile::corrupted;
	}

	auto result{ findPath(map) };

	for (const auto index : result.path)
	{
		map.at(index) = 'O';
	}

	printMap(map);
	std::println("Steps: {}", result.steps);
}

