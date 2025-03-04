module;

#include <algorithm>
#include <array>
#include <queue>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

export module KeyFinder;

export enum class Direction
{
	none,
	up,
	down,
	left,
	right,
};

export enum class Keypad
{
	directional,
	numeric,
};


export class KeyFinder
{
public:
	struct Node
	{
		int index{};
		Direction direction{};
		int tCost{};
		int hCost{};

		[[nodiscard]]
		bool operator== (const Node& other) const noexcept
		{
			return index == other.index
				&& direction == other.direction
				&& tCost == other.tCost;
		}
	};

	struct NodeComparer
	{
		[[nodiscard]]
		bool operator() (const Node& lhs, const Node& rhs) const noexcept
		{
			return lhs.hCost > rhs.hCost;
		}
	};

	struct NodeHasher
	{
		std::hash<int> hash{ std::hash<int>{} };

		[[nodiscard]]
		std::size_t operator() (const Node& node) const noexcept
		{
			auto h1{ hash(node.index) };
			auto h2{ hash(node.tCost) };

			return h1 + (h2 << 1);
		}
	};

	[[nodiscard]]
	std::vector<char> find_path(const char targetButton, const int startButton, const Keypad type)
	{
		const auto target{ key_location(targetButton, type) };
		const auto offKeypad{ '#' };

		std::priority_queue<Node, std::vector<Node>, NodeComparer> openList;
		std::unordered_set<int> seen;
		std::unordered_map<Node, Node, NodeHasher> pathNodes;

		Node current{ startButton, Direction::none, 0, 0 };
		openList.push(current);
		pathNodes[current];

		while (!openList.empty())
		{
			current = openList.top();
			openList.pop();

			if (current.index == target)
			{
				std::vector<char> path;

				while (current.index != startButton)
				{
					path.push_back(direction_to_char(current.direction));
					current = pathNodes[current];
				}

				std::ranges::reverse(path);
				return path;
			}

			for (const auto& [next, direction] : possible_directions(current.index))
			{
				if (seen.contains(next) || keypad(type)[next] == offKeypad)
				{
					continue;
				}

				auto tCost{ current.tCost + (direction == current.direction ? 1 : 2) };
				auto hCost{ tCost + manhattan_distance(current.index, target) };

				Node node{ next, direction, tCost, hCost };

				pathNodes[node] = current;
				openList.push(node);
			}
			seen.insert(current.index);
		}

		return {};
	}

	[[nodiscard]]
	int key_location(const char c, const Keypad type) const
	{
		switch (c)
		{
		case 'A':
			return type == Keypad::numeric ? m_keyNumericA : m_keyDirectionalA;

		case '^':
			return m_keyUP;

		case 'v':
			return m_keyDOWN;

		case '<':
			return m_keyLEFT;

		case '>':
			return m_keyRIGHT;

		case '0':
			return m_key0;

		case '1':
			return m_key1;

		case '2':
			return m_key2;

		case '3':
			return m_key3;

		case '4':
			return m_key4;

		case '5':
			return m_key5;

		case '6':
			return m_key6;

		case '7':
			return m_key7;

		case '8':
			return m_key8;

		case '9':
			return m_key9;

		}

		throw std::invalid_argument("Invalid Keypad Key: " + c);
	}


private:
	[[nodiscard]]
	char direction_to_char(const Direction direction) const
	{
		switch (direction)
		{
		case Direction::up:
			return m_charUP;

		case Direction::down:
			return m_charDOWN;

		case Direction::right:
			return m_charRIGHT;

		case Direction::left:
			return m_charLEFT;
		}

		throw std::invalid_argument("Invalid Direction: " + std::to_string(static_cast<int>(direction)));
	}

	[[nodiscard]]
	int manhattan_distance(const int current, const int target) const noexcept
	{
		auto currentX{ current % m_lineLength };
		auto currentY{ current / m_lineLength };
		auto targetX{ target % m_lineLength };
		auto targetY{ target / m_lineLength };

		return std::abs(currentX - targetX) + std::abs(currentY - targetY);
	}

	[[nodiscard]]
	std::array<std::pair<int, Direction>, 4> possible_directions(const int index) const noexcept
	{
		const auto up{ index - m_lineLength };
		const auto down{ index + m_lineLength };
		const auto left{ index - 1 };
		const auto right{ index + 1 };

		return { {
			 std::make_pair(up, Direction::up),
			 std::make_pair(down, Direction::down),
			 std::make_pair(left, Direction::left),
			 std::make_pair(right, Direction::right),
		} };
	}

	[[nodiscard]]
	std::string_view keypad(const Keypad type) const noexcept
	{
		return type == Keypad::directional ? m_directionalKeypad : m_numericKeypad;
	}

	const int m_lineLength{ 5 };

	const std::string_view m_directionalKeypad{
		"#####"
		"##^A#"
		"#<v>#"
		"#####"
	};

	const std::string_view m_numericKeypad{
		"#####"
		"#789#"
		"#456#"
		"#123#"
		"##0A#"
		"#####"
	};

	const char m_charUP    { '^' };
	const char m_charDOWN  { 'v' };
	const char m_charLEFT  { '<' };
	const char m_charRIGHT { '>' };
	const char m_charA     { 'A' };

	const int m_keyUP{ 7 };
	const int m_keyDOWN{ 12 };
	const int m_keyLEFT{ 11 };
	const int m_keyRIGHT{ 13 };
	const int m_keyDirectionalA{ 8 };

	const int m_key9{ 8 };
	const int m_key8{ 7 };
	const int m_key7{ 6 };
	const int m_key6{ 13 };
	const int m_key5{ 12 };
	const int m_key4{ 11 };
	const int m_key3{ 18 };
	const int m_key2{ 17 };
	const int m_key1{ 16 };
	const int m_key0{ 22 };
	const int m_keyNumericA{ 23 };
};

