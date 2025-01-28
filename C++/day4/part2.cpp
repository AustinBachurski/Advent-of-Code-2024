#include <algorithm>
#include <array>
#include <compare>
#include <numeric>
#include <fstream>
#include <iostream>
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
	Index five;
	auto operator<=>(const WordIndicies&) const = default;
};

static std::array<WordIndicies, 1> get_indicies(size_t row, size_t column)
{
	return { {
		{ { row, column }, { row + 1, column + 1 }, { row + 2, column + 2 }, { row, column + 2 }, { row + 2, column } },
		} };
}

static bool is_festive(const WordIndicies& indicies, const std::vector<std::string>& lines)
{
	return
		((
			lines[indicies.one.row][indicies.one.column] == 'M'
			&& lines[indicies.two.row][indicies.two.column] == 'A'
			&& lines[indicies.three.row][indicies.three.column] == 'S'
			)
		||
		(
			lines[indicies.three.row][indicies.three.column] == 'M'
			&& lines[indicies.two.row][indicies.two.column] == 'A'
			&& lines[indicies.one.row][indicies.one.column] == 'S'
			))
		&&
		((
			lines[indicies.four.row][indicies.four.column] == 'M'
			&& lines[indicies.two.row][indicies.two.column] == 'A'
			&& lines[indicies.five.row][indicies.five.column] == 'S'
			)
		||
		(
			lines[indicies.five.row][indicies.five.column] == 'M'
			&& lines[indicies.two.row][indicies.two.column] == 'A'
			&& lines[indicies.four.row][indicies.four.column] == 'S'
			));
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

	const size_t row_bounds = lines.size() - 3;
	const size_t column_bounds = lines.front().size() - 3;

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

