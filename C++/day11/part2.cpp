#include <algorithm>
#include <format>
#include <compare>
#include <numeric>
#include <fstream>
#include <iostream>
#include <print>
#include <ranges>
#include <set>
#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>
#include <vector>

struct PairHash {
	size_t operator() (const std::pair<uint64_t, int>& pair) const
	{
		return std::hash<uint64_t>{}(pair.first) ^ (std::hash<int>{}(pair.second) << 1);
	}
};

uint64_t calc(std::unordered_map<std::pair<uint64_t, int>, uint64_t, PairHash>& cache, uint64_t stone, int blinks)
{
	if (cache.contains({ stone, blinks }))
	{
		return cache[{ stone, blinks }];
	}

	if (!blinks)
	{
		return 1;
	}

	if (!stone)
	{
		auto result = calc(cache, 1, blinks - 1);
		cache.try_emplace({ stone, blinks }, result);
		return result;
	}

	auto value{ stone };
	uint64_t digits = 0;

	while (value)
	{
		value /= 10;
		++digits;
	}

	if (!(digits & 1))
	{
		uint64_t left{};
		uint64_t right{};
		uint64_t mul = 1;
		value = stone;

		for (const auto i : std::views::iota(static_cast<uint64_t>(0), digits / 2))
		{
			right += (value % 10) * mul;
			value /= 10;
			mul *= 10;

			if (i + 1 == digits / 2)
			{
				left = value;
			}
		}
		auto result = calc(cache, left, blinks - 1) + calc(cache, right, blinks - 1);
		cache.try_emplace({ stone, blinks }, result);
		return result;
	}

	auto result = calc(cache, stone * 2024, blinks - 1);
	cache.try_emplace({ stone, blinks }, result);
	return result;
}


int main()
{
	std::ifstream file{ "C:/dev/cpp/Advent of Code/day11/input.txt", std::ios::in };

	if (!file.is_open())
	{
		std::println("Failed to open input file.");
		return 1;
	}

	std::string line;
	std::getline(file, line);

	std::vector<uint64_t> data;

	uint64_t number{};

	for (const auto value : std::views::split(line, ' '))
	{
		std::from_chars(value.data(), value.data() + value.size(), number);
		data.push_back(number);
	}

	constexpr int blinks{ 75 };

	size_t result{};

	std::unordered_map<std::pair<uint64_t, int>, uint64_t, PairHash> cache;

	for (const auto stone : data)
	{
		
		result += calc(cache, stone, blinks);
	}

	std::println("{}", result);
}

