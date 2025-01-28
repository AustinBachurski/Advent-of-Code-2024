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

uint64_t concat(uint64_t total, uint64_t number)
{
	uint64_t num = number;
	uint64_t places = 10;

	while (num / 10 != 0)
	{
		places *= 10;
		num /= 10;
	}

	return total * places + number;
}

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

		for (uint32_t operation : std::views::iota(static_cast<uint32_t>(0), (std::numeric_limits<uint32_t>::max() >> (32 - operator_count * 2))))
		{
			uint64_t total = entry.values.front();
			uint64_t shift = 0;
			for (const auto value : entry.values)
			{
				if (((operation & (1 << shift)) == 0) && ((operation & (1 << (shift + 1))) == 0))
				{
					total += value;
				}
				else if (((operation & (1 << shift)) == 0) && ((operation & (1 << (shift + 1))) > 0))
				{
					total *= value;
				}
				else if (((operation & (1 << shift)) > 0) && ((operation & (1 << (shift + 1))) == 0))
				{
					total = concat(total, value);
				}
				else
				{
					total = 0;
					break;
				}

				shift += 2;
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

