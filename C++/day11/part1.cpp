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
#include <vector>

bool ruleOne(uint64_t stone, std::vector<uint64_t>& target)
{
	if (!stone)
	{
		target.push_back(1);
		return true;
	}
	return false;
}

bool ruleTwo(uint64_t stone, std::vector<uint64_t>& target)
{
	auto value{ stone };
	uint64_t digits = 0;

	while (value)
	{
		value /= 10;
		++digits;
	}

	if ((digits & 1))
	{ 
		return false;
	}

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

	target.push_back(left);
	target.push_back(right);

	return true;
}

void ruleThree(uint64_t stone, std::vector<uint64_t>& target)
{
	constexpr uint64_t special_value{ 2024 };
	target.push_back(stone * special_value);
}


std::vector<uint64_t>& mutate(std::vector<uint64_t>& source, std::vector<uint64_t>& target)
{
	target.clear();

	for (const auto stone : source)
	{
		if (!ruleOne(stone, target))
		{
			if (!ruleTwo(stone, target))
			{
				ruleThree(stone, target);
			}
		}
	}

	return target;
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

	std::vector<uint64_t> a;
	a.reserve(1'000'000);
	std::vector<uint64_t> b;
	b.reserve(1'000'000);

	uint64_t number{};

	for (const auto value : std::views::split(line, ' '))
	{
		std::from_chars(value.data(), value.data() + value.size(), number);
		a.push_back(number);
	}

	constexpr int blinks{ 25 };
	std::vector<uint64_t>& result = a;

	for (const auto blink : std::views::iota(0, blinks))
	{
		result = mutate(blink & 1 ? b : a, blink & 1 ? a : b);
	}


	std::println("{}", result.size());
}

