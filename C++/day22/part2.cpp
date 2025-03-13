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


struct Values
{
	Values() = default;
	Values(int start)
		: current{ start }
	{ }

	int current{};
	int previous{};
	int one{};
	int two{};
	int three{};
	int four{};

	int count{ 0 };

	void shuffle(const int number)
	{
		++count;

		previous = current;
		current = number;
		one = two;
		two = three;
		three = four;
		four = current - previous;
	}

	bool operator== (const Values& other) const noexcept
	{
		return one == other.one
			&& two == other.two
			&& three == other.three
			&& four == other.four;
	}
};

struct ValuesHash
{
	std::size_t operator() (const Values& value) const noexcept
	{
		std::hash<int> hash{ std::hash<int> {} };

		std::size_t h1{ hash(value.one) };
		std::size_t h2{ hash(value.two) };
		std::size_t h3{ hash(value.three) };
		std::size_t h4{ hash(value.four) };

		return h1 + (h2 << 1) + (h3 << 2) + (h4 << 3);
	}
};

std::size_t evolve(const std::size_t secretNumber, Values& values)
{
	auto result{ secretNumber * 64 };
	result ^= secretNumber;
	result %= 16777216;
	result ^= result / 32;
	result %= 16777216;
	result ^= result * 2048;
	result %= 16777216;

	values.shuffle(result % 10);

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

	std::mutex mutex;
	std::unordered_map<Values, std::size_t, ValuesHash> records;

	auto inputs{ std::views::split(input, '\n') };

	std::size_t count{};

	for (const auto line : inputs)
	{
		std::string_view secretString{ line };
		std::size_t secretNumber{};
		std::from_chars(secretString.data(), secretString.data() + secretString.size(), secretNumber);

		Values values{ secretNumber % 10 };

		for (const auto _ : std::views::iota(0, 2000))
		{
			secretNumber = evolve(secretNumber, values);

			if (values.count < 4)
			{
				continue;
			}

			if (records.contains(values))
			{
				continue;
			}

			records[values] = static_cast<std::size_t>(values.current);

			std::for_each(std::execution::par, inputs.begin(), inputs.end(), [&mutex, &records, &values, &secretString](const auto number)
				{
					std::string_view numberString{ number };

					if (secretString == numberString)
					{
						return;
					}

					std::size_t otherNumber{};
					std::from_chars(numberString.data(), numberString.data() + numberString.size(), otherNumber);

					Values intermediate{ otherNumber % 10 };

					for (const auto _ : std::views::iota(0, 2000))
					{
						otherNumber = evolve(otherNumber, intermediate);

						if (intermediate.count < 4)
						{
							continue;
						}

						if (values == intermediate)
						{
							const std::lock_guard<std::mutex> _{ mutex };
							records[intermediate] += static_cast<std::size_t>(intermediate.current);
							break;
						}
					}
				});
		}
	}

	auto max{ std::ranges::max_element(records, [](const std::pair<Values, std::size_t>& a, const std::pair<Values, std::size_t>& b)
		{
			return a.second < b.second;
		}) };

	auto x{ (*max).first };

	std::println("{}, {}, {}, {}", x.one, x.two, x.three, x.four);

	std::println("{}", (*max).second);

}

