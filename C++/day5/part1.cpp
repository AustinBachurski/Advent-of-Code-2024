#include <algorithm>
#include <numeric>
#include <fstream>
#include <print>
#include <ranges>
#include <string>
#include <string_view>
#include <vector>


int main()
{
	std::ifstream file{ "C:/dev/cpp/Advent of Code/day5/input.txt", std::ios::in };

	if (!file.is_open())
	{
		std::println("Failed to open input file.");
		return 1;
	}

	std::vector<std::pair<int, int>> ordering;
	std::vector<std::vector<int>> updates;

	std::string line;
	while (std::getline(file, line))
	{
		if (line.contains('|'))
		{
			auto split = std::views::split(line, '|');

			std::string_view x{ *split.begin() };
			std::string_view y{ *(++split.begin()) };

			int num = 0;
			int ber = 0;

			if (std::from_chars(x.data(), x.data() + x.size(), num).ec == std::errc{}
				&& std::from_chars(y.data(), y.data() + y.size(), ber).ec == std::errc{})
			{
				ordering.emplace_back(num, ber);
			}
		}

		if (line.contains(','))
		{
			std::vector<int> values;

			for (const auto each : std::views::split(line, ','))
			{
				int number = 0;
				std::string_view v{ each };

				if (std::from_chars(v.data(), v.data() + v.size(), number).ec == std::errc{})
				{
					values.push_back(number);
				}
			}

			updates.push_back(std::move(values));
		}
	}

	int result = 0;
	bool invalid = false;

	for (const auto& update : updates)
	{
		invalid = false;
		for (const int value : update)
		{
			auto order = ordering | std::views::filter([value](const std::pair<int, int> pair) {
				return pair.first == value;
				});

			for (auto [_, position] : order)
			{
				auto value_pos = std::ranges::find(update, value);
				auto order_pos = std::ranges::find(update, position);

				if (value_pos > order_pos)
				{
					invalid = true;
					break;
				}
			}
			if (invalid) { break; }
		}

		if (!invalid)
		{
			result += update[update.size() / 2];
		}
	}

	std::println("{}", result);

}
