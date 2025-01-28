#include <algorithm>
#include <compare>
#include <numeric>
#include <fstream>
#include <print>
#include <ranges>
#include <set>
#include <string>
#include <string_view>
#include <vector>


struct Index
{
	size_t row{};
	size_t column{};

	Index operator+(const Index& other) const
	{
		return { row + other.row, column + other.column };
	}

	Index operator-(const Index& other) const
	{
		return { row - other.row, column - other.column };
	}

	auto operator<=>(const Index&) const = default;
};

std::vector<Index> find_nodes(const Index index, char c, const std::vector<std::string>& lines)
{
	std::vector<Index> matches;

	for (const auto [row, line] : std::views::enumerate(lines))
	{
		size_t start = 0;
		size_t i = line.find(c, start);

		while (i != std::string::npos)
		{
			matches.emplace_back(row, i);
			i = line.find(c, i + 1);
		}
	}

	return matches;
}

bool is_valid(const Index index, const Index max)
{
	return index.row < max.row && index.column < max.column;
}

void set_antinodes(const Index max, const Index origin, const std::vector<Index>& nodes, std::set<Index>& antinodes)
{
	Index stride;
	Index target;

	for (const auto& node : nodes)
	{
		if (node == origin)
		{
			continue;
		}

		stride = origin - node;
		target = origin + stride;

		if (is_valid(target, max))
		{
			antinodes.insert(target);
		}


		target = node - stride;

		if (is_valid(target, max))
		{
			antinodes.insert(target);
		}
	}
}


int main()
{
	std::ifstream file{ "C:/dev/cpp/Advent of Code/day8/input.txt", std::ios::in };

	if (!file.is_open())
	{
		std::println("Failed to open input file.");
		return 1;
	}

	std::vector<std::string> lines;

	while (std::getline(file, lines.emplace_back()))
	{ }
	lines.pop_back();

	const Index max{ lines.size(), lines.front().size() };

	std::set<Index> antinodes;

	for (const auto [row, line] : std::views::enumerate(lines))
	{
		for (const auto [column, c] : std::views::enumerate(line))
		{
			if (c != '.')
			{
				auto nodes = find_nodes({ static_cast<size_t>(row), static_cast<size_t>(column) }, c, lines);
				set_antinodes(max, { static_cast<size_t>(row), static_cast<size_t>(column) }, nodes, antinodes);
			}
		}
	}

	std::println("{}", antinodes.size());
}

