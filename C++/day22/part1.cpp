#include <algorithm>
#include <chrono>
#include <execution>
#include <format>
#include <compare>
#include <fstream>
#include <iostream>
#include <memory>
#include <mutex>
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


std::size_t evolve(const std::size_t secretNumber)
{
	auto result{ secretNumber * 64 };
	result ^= secretNumber;
	result %= 16777216;
	result ^= result / 32;
	result %= 16777216;
	result ^= result * 2048;
	result %= 16777216;

	return result;
}


int main()
{
	std::ifstream file{ "C:/dev/cpp/Advent of Code/day22/input.txt", std::ios::in };

	if (!file.is_open())
	{
		std::println("Failed to open input file.");
		return 1;
	}

	std::string input{ std::istreambuf_iterator{ file }, {} };

	std::size_t total{};

	for (const auto line : std::views::split(input, '\n'))
	{
		std::string_view secretString{ line };
		std::size_t secretNumber{};

		std::from_chars(secretString.data(), secretString.data() + secretString.size(), secretNumber);

		for (const auto _ : std::views::iota(0, 2000))
		{
			secretNumber = evolve(secretNumber);
		}

		total += secretNumber;
	}
	std::println("{}", total);
}

