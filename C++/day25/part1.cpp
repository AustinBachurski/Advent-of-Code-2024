#include <algorithm>
#include <thread>
#include <chrono>
#include <deque>
#include <array>
#include <fstream>
#include <iostream>
#include <iterator>
#include <print>
#include <queue>
#include <ranges>
#include <stdexcept>
#include <unordered_set>
#include <map>
#include <vector>


constexpr auto tumblerCount{ 5 };
constexpr auto keyDepth{ 7 };

static auto get_values(std::string_view const schematic) -> std::array<unsigned int, tumblerCount>
{
	std::array<unsigned int, tumblerCount> tumblers{};

	auto removeNewlines{ [](char const c) { return c != '\n'; } };

	for (auto const &[index, value] : schematic
									| std::views::filter(removeNewlines)
									| std::views::enumerate)
	{
		if (value == '#')
		{
			++tumblers[index % tumblerCount];
			auto x = 0;
		}
	}

	return tumblers;
}

template <typename T>
concept StringViewable = requires(T s)
{
	std::string_view{ *s.begin() };
};

template <StringViewable T>
auto read_schematics(T schematics,
	std::vector<std::array<unsigned int, tumblerCount>> &locks,
	std::vector<std::array<unsigned int, tumblerCount>> &keys) -> void
{
	for (auto const schematic : schematics)
	{
		std::string_view const schematicText{ schematic };

		if (schematicText.starts_with('.'))
		{
			keys.push_back(get_values(schematicText));
		}
		else if (schematicText.starts_with('#'))
		{
			locks.push_back(get_values(schematicText));
		}
	}
}


int main()
{
	std::ifstream file("C:/dev/cpp/Advent of Code/day25/input.txt", std::ios::in);

	if (!file.is_open())
	{
		std::println("Failed to open input file.");
		return 1;
	}

	std::string const input(std::istreambuf_iterator<char>{file}, {});

	using namespace std::string_view_literals;
	auto schematics{ std::views::split(input, "\n\n"sv) };

	std::vector<std::array<unsigned int, tumblerCount>> locks;
	std::vector<std::array<unsigned int, tumblerCount>> keys;

	read_schematics(schematics, locks, keys);

	std::size_t total{ 0 };

	for (auto const &lock : locks)
	{
		total += std::ranges::count_if(keys, [&lock](auto const &key)
			{
				auto both{ std::views::zip(lock, key) };

				for (auto const &[l, k] : both)
				{
					if (l + k > keyDepth)
					{
						return false;
					}
				}
				return true;
			});
	}

	std::println("{}", total);
}

