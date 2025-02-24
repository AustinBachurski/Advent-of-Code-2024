#include <array>
#include <compare>
#include <fstream>
#include <iostream>
#include <print>
#include <queue>
#include <ranges>
#include <set>
#include <stack>
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <utility>

namespace mapTile
{
	constexpr char start{ 'S' };
	constexpr char end  { 'E' };
	constexpr char wall { '#' };
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
	std::size_t cost{};

	auto operator== (const Node& other) const noexcept
    {
        return other.index == index
            && other.direction == direction;
    }
};

struct NodeHasher
{
	[[nodiscard]]
	std::size_t operator() (const Node& node) const noexcept
	{
		auto h1{ std::hash<std::size_t>{}(node.index) };
		auto h2{ std::hash<Direction>{}(node.direction) };

		return h1 ^ (h2 << 1);
	}
};

struct NodeCompare
{
	[[nodiscard]]
	bool operator() (const Node& lhs, const Node& rhs) const noexcept
	{
		return lhs.cost > rhs.cost;
	}
};

struct NextTile
{
	std::size_t index{};
	Direction direction{};
};

constexpr std::array<NextTile, 3> get_next_moves(const std::size_t index, const Direction direction, const std::size_t lineLength)
{
	std::array<NextTile, 3> moves{};

	const auto north{ index - lineLength };
	const auto south{ index + lineLength };
	const auto east{ index + 1 };
	const auto west{ index - 1 };

	switch (direction)
	{
	case Direction::north:
		moves = { { { north, direction }, { east, Direction::east }, { west, Direction::west } } };
		break;

	case Direction::south:
		moves = { { { south, direction }, { east, Direction::east }, { west, Direction::west } } };
		break;

	case Direction::east:
		moves = { { { east, direction }, { north, Direction::north }, { south, Direction::south } } };
		break;

	case Direction::west:
		moves = { { { west, direction} , { north, Direction::north }, { south, Direction::south } } };
		break;
	}

	return moves;
}

std::pair<std::size_t, std::set<std::size_t>> find_best_path(const std::string_view map, const std::size_t start, const std::size_t goal)
{
	const std::size_t lineLength = map.find('\n') + 1;

	std::size_t bestCost{ std::numeric_limits<std::size_t>::max() };
	std::priority_queue<Node, std::vector<Node>, NodeCompare> toVisit;
	std::unordered_set<Node, NodeHasher> seen;
	std::unordered_map<Node, std::vector<Node>, NodeHasher> path;
	std::stack<Node> pathNodes;

	toVisit.emplace(start, Direction::east, 0);
    seen.emplace(start, Direction::east, 0);

	while (!toVisit.empty())
	{
		auto current{ toVisit.top() };
		toVisit.pop();

		if (goal == current.index)
		{
            if (bestCost < current.cost)
            {
                break;
            }

            bestCost = current.cost;
			pathNodes.push(current);
			continue;
		}

        auto nextMoves{ get_next_moves(current.index, current.direction, lineLength) };

		for (const auto [turning, next] : std::views::enumerate(nextMoves))
		{
			if (map.at(next.index) == mapTile::wall)
			{
				continue;
			}

			const auto cost{ turning ? current.cost + 1001 : current.cost + 1 };

			if (cost > bestCost)
			{
				continue;
			}

            Node node{ next.index, next.direction, cost};

            if (const auto prior{ seen.find(node) }; prior != seen.end())
            {
                if (prior->cost > node.cost)
                {
                    // Found a cheaper route, search again.
                    seen.erase(prior);
                    seen.insert(node);
                    path[node] = { current };
                    toVisit.push(node);
                }
                else if (prior->cost == node.cost)
                {
                    // Additional route with same cost, good seats.
                    path[node].push_back(current);
                    seen.insert(node);
                    continue;
                }
                else
                {
                    seen.insert(node);
                    continue;
                }
            }
			else
			{
				path[node] = { current };
				seen.insert(node);
				toVisit.push(node);
			}
		}
	}

	std::set<std::size_t> seats;
	seats.insert(pathNodes.top().index);

	Node pathing;

	while (!pathNodes.empty())
	{
		pathing = pathNodes.top();
		pathNodes.pop();
		for (const auto& each : path[pathing])
		{
			seats.insert(each.index);
			pathNodes.push(each);
		}
	}

	return std::make_pair(bestCost, seats);
}

int main()
{
	std::ifstream file{ "C:/dev/cpp/Advent of Code/day16/input.txt", std::ios::in };

	if (!file.is_open())
	{
		std::println("Failed to open input file.");
	}

	std::string input{ std::istreambuf_iterator<char>{file}, {} };

	auto best{ find_best_path(input, input.find(mapTile::start), input.find(mapTile::end)) };

	for (const auto i : best.second)
	{
		input.at(i) = 'O';
	}

	// Make map easier to see.
	for (char& each : input)
	{
		if (each == '.')
		{
			each = ' ';
		}
	}

	std::println("{}", input);
	std::println("Cost: {}", best.first);
	std::println("Seats: {}", best.second.size());
}
