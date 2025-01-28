#include <algorithm>
#include <chrono>
#include <execution>
#include <fstream>
#include <numeric>
#include <print>
#include <ranges>
#include <string>
#include <string_view>
#include <tuple>
#include <vector>


struct Data
{
	uint64_t target = 0;
	std::vector<uint64_t> values;
};

int main()
{
	std::ifstream file{ "C:/dev/cpp/Advent of Code/day7/input.txt", std::ios::in };

	if (!file.is_open())
	{
		std::println("Failed to open input file.");
		return 1;
	}

	std::string line;
	std::vector<Data> data;

	while (std::getline(file, line))
	{
		auto split = std::views::split(line, ':');

		std::string_view left{ split.front() };
		Data entry;

		std::from_chars(left.data(), left.data() + left.size(), entry.target);

		uint64_t number = 0;

		for (auto each : std::views::split(*(++split.begin()), ' '))
		{
			if (std::from_chars(each.data(), each.data() + each.size(), number).ec == std::errc{})
			{
				entry.values.push_back(number);
			}
		}
		
		data.push_back(std::move(entry));
	}

	uint64_t result = 0;
	size_t operator_count = 0;

	for (const auto& entry : data)
	{
		operator_count = entry.values.size() - 1;

		for (uint16_t operation : std::views::iota(0, std::numeric_limits<uint16_t>::max() >> (16 - operator_count - 1)))
		{
			uint64_t total = entry.values.front();
			for (const auto [shift, value] : std::views::enumerate(entry.values) | std::views::drop(1))
			{
				if ((operation & (1 << (operator_count - shift))) == 0)
				{
					total += value;
				}
				else
				{
					total *= value;
				}
			}

			if (entry.target == total)
			{
				result += entry.target;
				break;
			}
		}
	}

	std::println("{}", result);

}

