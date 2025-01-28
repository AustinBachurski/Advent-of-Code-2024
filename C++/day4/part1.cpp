#include <algorithm>
#include <array>
#include <compare>
#include <numeric>
#include <fstream>
#include <print>
#include <ranges>
#include <regex>
#include <set>
#include <string>
#include <string_view>
#include <vector>

struct Index
{
	size_t row;
	size_t column;
	auto operator<=>(const Index&) const = default;
};

struct WordIndicies
{
	Index one;
	Index two;
	Index three;
	Index four;
	auto operator<=>(const WordIndicies&) const = default;
};

static std::array<WordIndicies, 10> get_indicies(size_t row, size_t column)
{
	return { {
			// Horizontal
			{ { row, column }, { row, column + 1 }, { row, column + 2 }, { row, column + 3 } },
			{ { row + 1, column }, { row + 1, column + 1 }, { row + 1, column + 2 }, { row + 1, column + 3 } },
			{ { row + 2, column }, { row + 2, column + 1 }, { row + 2, column + 2 }, { row + 2, column + 3 } },
			{ { row + 3, column }, { row + 3, column + 1 }, { row + 3, column + 2 }, { row + 3, column + 3 } },

			// Vertical
			{ { row, column }, { row + 1, column }, { row + 2, column }, { row + 3, column } },
			{ { row, column + 1 }, { row + 1, column + 1 }, { row + 2, column + 1 }, { row + 3, column + 1 } },
			{ { row, column + 2 }, { row + 1, column + 2 }, { row + 2, column + 2 }, { row + 3, column + 2 } },
			{ { row, column + 3 }, { row + 1, column + 3 }, { row + 2, column + 3 }, { row + 3, column + 3 } },

			// Diaganal
			{ { row, column }, { row + 1, column + 1 }, { row + 2, column + 2 }, { row + 3, column + 3 } },
			{ { row, column + 3 }, { row + 1, column + 2 }, { row + 2, column + 1 }, { row + 3, column } },
		} };
}

static bool is_festive(const WordIndicies& indicies, const std::vector<std::string>& lines)
{
	return
		// Forwards
		(
			lines[indicies.one.row][indicies.one.column] == 'X'
			&& lines[indicies.two.row][indicies.two.column] == 'M'
			&& lines[indicies.three.row][indicies.three.column] == 'A'
			&& lines[indicies.four.row][indicies.four.column] == 'S'
			)
		||
		// Backwards
		(
			lines[indicies.four.row][indicies.four.column] == 'X'
			&& lines[indicies.three.row][indicies.three.column] == 'M'
			&& lines[indicies.two.row][indicies.two.column] == 'A'
			&& lines[indicies.one.row][indicies.one.column] == 'S'
			);
}


int main()
{
	std::ifstream file{ "C:/dev/cpp/Advent of Code/day4/input.txt", std::ios::in };

	if (!file.is_open())
	{
		std::println("Failed to open input file.");
		return 1;
	}

	std::vector<std::string> lines;

	while (std::getline(file, lines.emplace_back()))
	{ }
	lines.pop_back();

	std::set<WordIndicies> count;

	const size_t row_bounds = lines.size() - 4;
	const size_t column_bounds = lines.front().size() - 4;

	size_t row = 0;
	size_t column = 0;

	while (row <= row_bounds)
	{
		while (column <= column_bounds)
		{
			auto indicies = get_indicies(row, column);

			for (const auto& each : indicies)
			{
				if (is_festive(each, lines))
				{
					count.insert(each);
				}
			}

			++column;
		}
		++row;
		column = 0;
	}

	std::println("{}", count.size());
}

