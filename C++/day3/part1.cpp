#include <algorithm>
#include <numeric>
#include <fstream>
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

	const std::regex pattern{ "mul\\(([0-9][0-9]?[0-9]?,[0-9][0-9]?[0-9]?)\\)" };

	int result = 0;

	std::string line;

	while (std::getline(file, line))
	{
		auto matches = std::sregex_iterator(line.begin(), line.end(), pattern);

		auto end = std::sregex_iterator();

		for (const auto& match : std::ranges::subrange(matches, end))
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

	}

	std::println("{}", result);

}
