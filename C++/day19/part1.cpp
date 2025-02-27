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


auto get_index_matches(const std::string_view design, const std::set<std::string_view>& towels)
						-> std::unordered_map<std::size_t, std::vector<std::string_view>>
{
	std::unordered_map<std::size_t, std::vector<std::string_view>> matches;

	for (const auto towel : towels)
	{
		if (auto index{ design.find(towel) }; std::string::npos != index)
		{
			while (std::string::npos != index)
			{
				matches[index].push_back(towel);
				index = design.find(towel, index + 1);
			}
		}
	}
	return matches;
}

bool can_complete(const std::string_view design, const std::size_t index, const std::unordered_map<std::size_t, std::vector<std::string_view>>& matches)
{
	if (index == design.size())
	{
		return true;
	}

	if (!matches.contains(index))
	{
		return false;
	}

	for (const auto towel : matches.at(index))
	{
		if (can_complete(design, index + towel.size(), matches))
		{
			return true;
		}
	}

	return false;
}

bool can_make_design(const std::string_view design, const std::set<std::string_view>& towels)
{
	const auto matches = get_index_matches(design, towels);

	const std::size_t index{ 0 };

	if (!matches.contains(index))
	{
		return false;
	}

	for (const auto towel : matches.at(index))
	{
		if (can_complete(design, index + towel.size(), matches))
		{
			return true;
		}
	}

	return false;
}

bool canRemove(const std::string_view maybeRemove, const std::set<std::string_view>& towels)
{
	constexpr auto isWhitespace{ [](const char c) { return c == ' '; } };

	std::string current{ maybeRemove };

	for (const auto towel : towels)
	{
		if (towel == maybeRemove)
		{
			continue;
		}

		std::size_t start{ maybeRemove.find(towel) };

		while (std::string::npos != start)
		{
			current.replace(start, towel.size(), std::string(towel.size(), ' '));
			start = maybeRemove.find(towel, start + 1);
		}

		if (std::ranges::all_of(current, isWhitespace))
		{
			return true;
		}
	}
	return false;
}

std::vector<std::string_view> simplify(const std::set<std::string_view>& towels)
{
	std::vector<std::string_view> removals;

	for (const auto towel : towels)
	{
		if (canRemove(towel, towels))
		{
			removals.push_back(towel);
		}
	}

	return removals;
}

int main()
{
	std::ifstream file{ "C:/dev/cpp/Advent of Code/day19/input.txt", std::ios::in };

	if (!file.is_open())
	{
		std::println("Failed to open input file.");
		return 1;
	}

	std::string available;
	std::getline(file, available);

	std::set<std::string_view> towels;

	using namespace std::string_view_literals;
	for (const auto towel : std::views::split(available, ", "sv))
	{
		towels.emplace(towel);
	}

	{
		auto removals{ simplify(towels) };
		for (const auto towel : removals)
		{
			towels.erase(towel);
		}
	}

	std::string design;
	std::getline(file, design); // Ignore empty line.

	int possible{};

	while (std::getline(file, design))
	{
		if (can_make_design(design, towels))
		{
			++possible;
		}
	}

	std::println("Possible: {}", possible);
}

