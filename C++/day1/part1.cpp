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
	std::ifstream file{ "C:/dev/cpp/Advent of Code/day1/input.txt", std::ios::in };

	if (!file.is_open())
	{
		std::println("Failed to open input file.");
		return 1;
	}

	std::vector<int> left;
	std::vector<int> right;

	std::string line;

	while (std::getline(file, line))
	{
		auto split = std::views::split(line, ' ');

		std::string_view x{ *split.begin() };
		std::string_view y{ *(++++++split.begin()) };

		int number = 0;

		if (std::from_chars(x.data(), x.data() + x.size(), number).ec == std::errc{})
		{
			left.push_back(number);
		}

		if (std::from_chars(y.data(), y.data() + y.size(), number).ec == std::errc{})
		{
			right.push_back(number);
		}
	}

	std::ranges::sort(left);
	std::ranges::sort(right);

	for (size_t i = 0; i < left.size(); ++i)
	{
		left[i] = std::abs(right[i] - left[i]);
	}

	std::println("{}", std::accumulate(left.begin(), left.end(), 0));

}