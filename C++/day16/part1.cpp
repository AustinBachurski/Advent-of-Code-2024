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
#include <string>
#include <string_view>
#include <vector>


namespace mapTile
{
	constexpr char start { 'S' };
	constexpr char empty { '.' };
	constexpr char end   { 'E' };
	constexpr char wall  { '#' };
}


class Mazesolver
{
public:
	Mazesolver(const std::string_view map)
		: m_lineLength{ map.find('\n') + 1 }
		, m_map{ map }
	{ }

	std::vector<size_t> navigate()
	{
		return find_path(m_map.find(mapTile::start), m_map.find(mapTile::end));
	}

private:
	enum class Direction
	{
		vertial = 0,
		horizontal = 1,
	};

	struct Node
	{
	public:
		Node(const size_t index,
			const Direction directionOfTravel,
			std::weak_ptr<Node> parent,
			const size_t travelCost,
			const double heuristic)
			: index{ index }
			, direction{ directionOfTravel }
			, parent{ parent }
			, nodeCost{ travelCost }
			, hCost{ heuristic }
		{ }

		size_t index;
		Direction direction;
		std::weak_ptr<Node> parent;
		size_t nodeCost;
		double hCost;
	};

	std::vector<size_t> find_path(const size_t startIndex, const size_t targetIndex)
	{
		struct compare
		{
			bool operator() (const std::shared_ptr<Node>& a, const std::shared_ptr<Node>& b) const
			{
				return a->hCost > b->hCost;
			}
		};

		std::priority_queue<std::shared_ptr<Node>, std::vector<std::shared_ptr<Node>>, compare> openList;
		std::vector<std::shared_ptr<Node>> closedList;

		openList.emplace(std::make_shared<Node>(startIndex, Direction::horizontal, std::weak_ptr<Node>{}, 0, 0));

		std::shared_ptr<Node> current;

		while (!openList.empty())
		{
			current = openList.top();
			openList.pop();

			if (current->index == targetIndex)
			{
				std::println("Cost: {}", current->nodeCost);
				std::vector<size_t> path;

				while (!current->parent.expired())
				{
					path.push_back(current->index);
					current = current->parent.lock();
				}

				return path;
			}

			for (const auto neighbor : m_neighbors)
			{
				size_t nodeIndex{ current->index + neighbor };

				auto containsThisNode{ [nodeIndex](std::shared_ptr<Node>& node) { return node->index == nodeIndex; } };
				if (std::ranges::any_of(closedList, containsThisNode))
				{ 
					continue;
				}

				auto tile{ m_map.at(nodeIndex) };

				if (tile == mapTile::wall) { continue; }

				Direction nodeDirection{ static_cast<Direction>(std::abs(neighbor) == 1) };
				auto nodeCost{ calculate_cost(current, nodeDirection) };
				auto hCost{ distance_between(nodeIndex, targetIndex) + nodeCost };

				openList.emplace(std::make_shared<Node>(nodeIndex, nodeDirection, current, nodeCost, hCost));
			}

			closedList.push_back(std::move(current));
		}

		return {};
	}

	size_t calculate_cost(std::shared_ptr<Node>& source, Direction nodeDirection) const
	{
		size_t travelCost{ static_cast<size_t>(source->direction == nodeDirection ? 1 : 1001) };
		return source->nodeCost + travelCost;
	}

	double distance_between(const size_t begin, const size_t end) const
	{
		double dx{ static_cast<double>((begin % m_lineLength)) - static_cast<double>((end % m_lineLength)) };
		double dy{ static_cast<double>((begin / m_lineLength)) - static_cast<double>((end / m_lineLength)) };

		return std::sqrt(dx * dx + dy * dy);
	}

	size_t m_lineLength;
	std::string_view m_map;
	std::array<int, 4> m_neighbors { 1, -1, static_cast<int>(m_lineLength), -static_cast<int>(m_lineLength) };
};

int main()
{
	std::ifstream file{ "C:/dev/cpp/Advent of Code/day16/input.txt", std::ios::in };

	if (!file.is_open())
	{
		std::println("Failed to open input file.");
		return 1;
	}

	std::string input{ std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>() };

	Mazesolver maze(input);
	auto path = maze.navigate();

	// Easier to see the path.
	for (char& each : input)
	{
		if (each == mapTile::empty)
		{
			each = ' ';
		}
	}

	for (const auto index : path | std::views::drop(1))
	{
		input.at(index) = '*';
	}

	std::println("{}", input);
}

