#include <algorithm>
#include <numeric>
#include <fstream>
#include <print>
#include <ranges>
#include <string>
#include <string_view>
#include <vector>

struct Index
{
	int64_t row;
	int64_t column;

	auto operator+=(const Index& other)
	{
		row += other.row;
		column += other.column;
	}

	auto operator-=(const Index& other)
	{
		row -= other.row;
		column -= other.column;
	}
};

class Pathing
{
public:
	Pathing(Index guard) : pos{ guard } {}

	void turn()
	{
		pos -= move;

		switch (direction)
		{
		case Direction::up:
		{
			move = right;
			direction = Direction::right;
			break;
		}

		case Direction::right:
		{
			move = down;
			direction = Direction::down;
			break;
		}

		case Direction::down:
		{
			move = left;
			direction = Direction::left;
			break;
		}

		case Direction::left:
		{
			move = up;
			direction = Direction::up;
			break;
		}
		}
	}

	void next()
	{
		pos += move;
	}

	Index pos;

private:
	enum class Direction
	{
		up,
		right,
		down,
		left,
	};

	Direction direction{ Direction::up };
	Index move = { -1, 0 };

	Index up{ -1, 0 };
	Index right{ 0, 1 };
	Index down{ 1, 0 };
	Index left{ 0, -1 };
};

Index find_guard(const std::vector<std::string>& lines)
{
	int64_t row = 0;

	for (const auto& line : lines)
	{
		if (line.contains('^'))
		{
			return { row, static_cast<int64_t>(line.find('^')) };
		}
		++row;
	}
}

int main()
{
	std::ifstream file{ "C:/dev/cpp/Advent of Code/day6/input.txt", std::ios::in };

	if (!file.is_open())
	{
		std::println("Failed to open input file.");
		return 1;
	}

	std::vector<std::string> lines;

	while (std::getline(file, lines.emplace_back()))
	{ }
	lines.pop_back();

	Pathing path{ find_guard(lines) };

	while (true)
	{
		// TODO: Stuck looping I think?
		lines[path.pos.row][path.pos.column] = 'X';
		path.next();

		try
		{
			if (lines.at(path.pos.row).at(path.pos.column) == '#')
			{
				path.turn();
			}
		}
		catch(std::out_of_range)
		{
			break;
		}
	}

	int64_t result = 0;

	for (const auto& line : lines)
	{
		result += std::ranges::count_if(line, [](const char c)
			{
				return c == 'X';
			});
	}

	std::println("{}", result);

}

