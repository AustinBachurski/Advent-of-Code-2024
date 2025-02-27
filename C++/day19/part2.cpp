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


std::size_t count_designs(std::unordered_map<std::string_view, std::size_t>& cache, const std::string_view design, const std::vector<std::string_view>& towels)
{
	if (cache.contains(design))
	{
		return cache[design];
	}

	if (design.empty())
	{
		return 1;
	}

	std::size_t count{};

	for (const auto towel : towels)
	{
		if (design.starts_with(towel))
		{
			count += count_designs(cache, design.substr(towel.size()), towels);
		}
	}
	cache[design] = count;

	return count;
}

int main()
{
	std::ifstream file{ "C:/dev/cpp/Advent of Code/day19/input.txt", std::ios::in };

	if (!file.is_open())
	{
		std::println("Failed to open input file.");
		return 1;
	}

	std::string input{ std::istreambuf_iterator<char>{ file }, {} };

	using namespace std::string_view_literals;
	auto towelsAndDesigns{ std::views::split(input, "\n\n"sv) };

	std::vector<std::string_view> towels;

	for (const auto towel : std::views::split(*towelsAndDesigns.begin(), ", "sv))
	{
		towels.emplace_back(towel);
	}

	std::vector<std::string_view> designs;

	for (const auto design : std::views::split(*++towelsAndDesigns.begin(), '\n'))
	{
		designs.emplace_back(design);
	}

	std::unordered_map<std::string_view, std::size_t> cache;
	std::size_t possible{};

	for (const auto design : designs)
	{
		possible += count_designs(cache, design, towels);
	}

	std::println("Possible: {}", possible);
}

