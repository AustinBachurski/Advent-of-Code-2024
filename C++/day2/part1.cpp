#include <algorithm>
#include <numeric>
#include <fstream>
#include <print>
#include <ranges>
#include <string>
#include <string_view>
#include <vector>

bool is_safe(const std::vector<int>& report)
{
	auto index = report.begin();

	int expected = 0;

	int previous = *index;
	int current = 0;
	++index;

	while (index != report.end())
	{
		current = *index;

		// Does step.
		if (current == previous)
		{
			return false;
		}

		// Within step bounds.
		if (std::abs(previous - current) > 3)
		{
			return false;
		}

		// Set direction.
		if (expected == 0)
		{
			expected = previous - current;
			previous = current;
			++index;
			continue;
		}

		// Same direction.
		if ((previous - current) > 0 != expected > 0)
		{
			return false;
		}

		previous = current;
		++index;
	}

	return true;
}

int main()
{
	std::ifstream file{ "C:/dev/cpp/Advent of Code/day2/input.txt", std::ios::in };

	if (!file.is_open())
	{
		std::println("Failed to open input file.");
		return 1;
	}

	std::string line;

	std::vector<std::vector<int>> reports;

	while (std::getline(file, line))
	{
		auto split = std::views::split(line, ' ');

		std::vector<int> section;

		int number = 0;

		for (const auto each : split)
		{
			if (std::from_chars(each.data(), each.data() + each.size(), number).ec == std::errc{})
			{
				section.push_back(number);
			}
		}

		reports.push_back(section);
	}

	int safe_count = 0;

	for (const auto& report : reports)
	{
		if (is_safe(report))
		{
			++safe_count;
		}
	}

	std::println("{}", safe_count);

}

