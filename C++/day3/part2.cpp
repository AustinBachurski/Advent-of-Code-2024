#include <algorithm>
#include <numeric>
#include <fstream>
#include <iostream>
#include <print>
#include <ranges>
#include <regex>
#include <string>
#include <string_view>
#include <vector>


int main()
{
	std::ifstream file{ "C:/dev/cpp/Advent of Code/day3/input.txt", std::ios::in };

	if (!file.is_open())
	{
		std::println("Failed to open input file.");
		return 1;
	}

	std::string input{ std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>() };

	size_t start = 0;
	size_t position = content.find("don't()", start);
	size_t end = content.find("do()", position);

	while (position != std::string::npos)
	{
		content.replace(position, end - position, "");

		position = content.find("don't()", start);
		end = content.find("do()", position);
	}

	const std::regex pattern{ "mul\\(([0-9][0-9]?[0-9]?,[0-9][0-9]?[0-9]?)\\)" };

	int result = 0;

	auto matches = std::sregex_iterator(content.begin(), content.end(), pattern);

	auto end_matches = std::sregex_iterator();

	for (const auto& match : std::ranges::subrange(matches, end_matches))
	{
		auto split = std::views::split(match[1].str(), ',');

		int temp = 1;
		int number = 0;

		for (const auto each : split)
		{
			if (std::from_chars(each.data(), each.data() + each.size(), number).ec == std::errc{})
			{
				temp *= number;
			}
		}

		result += temp;
	}

	std::println("{}", result);

}

