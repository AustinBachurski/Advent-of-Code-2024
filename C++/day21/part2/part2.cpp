import Keypads;

#include <deque>
#include <fstream>
#include <iterator>
#include <mdspan>
#include <print>
#include <ranges>
#include <string>
#include <string_view>
#include <type_traits>
#include <unordered_map>
#include <utility>
#include <vector>


struct CacheHash
{
	std::size_t operator() (const std::pair<std::string, int>& pair) const noexcept
	{
		auto h1{ std::hash<std::string>{}(pair.first) };
		auto h2{ std::hash<int>{}((pair.second)) };

		return h1 + (h2 << 1);
	}
};

std::size_t path_length(
	std::unordered_map<std::pair<char, char>, std::vector<std::string>, Keypad::PairHash>& possibleMoves, 
	std::unordered_map<std::pair<std::string, int>, std::size_t, CacheHash>& cache,
	const std::string& moves,
	const int robots = 25)
{
	const auto key{ std::make_pair(moves, robots) };

	if (cache.contains(key))
	{
		return cache[key];
	}

	if (robots == 1)
	{
		std::size_t sum{ 0 };

		for (const auto& [stepA, stepB] : std::views::zip('A' + moves, moves))
		{
			sum += possibleMoves[std::make_pair(stepA, stepB)].front().size();
		}

		cache[key] = sum;
		return sum;
	}

	std::size_t total{ 0 };

	for (const auto& [stepA, stepB] : std::views::zip('A' + moves, moves))
	{
		std::vector<std::size_t> lengths;

		for (const auto& moveSet : possibleMoves[std::make_pair(stepA, stepB)])
		{
			lengths.push_back(path_length(possibleMoves, cache, moveSet, robots - 1));
		}

		if (auto minIterator{ std::ranges::min_element(lengths) }; minIterator != lengths.end())
		{
			total += *minIterator;
		}
	}

	cache[key] = total;
	return total;
}

std::vector<std::string> cartesian_product(const std::vector<std::vector<std::string>>& collections, const std::size_t index = 0)
{
	std::vector<std::string> data;

	if (collections.empty())
	{
		return data;
	}

	if (index == collections.size() - 1)
	{
		for (const auto& element : collections[index])
		{
			data.push_back(element);
		}

		return data;
	}

	for (const auto& entry : cartesian_product(collections, index + 1))
	{
		for (const auto& element : collections[index])
		{
			data.push_back(element + entry);
		}
	}

	return data;
}


int main()
{
	std::ifstream file{ "C:/dev/cpp/Advent of Code/day21/input.txt", std::ios::in };

	if (!file.is_open())
	{
		std::println("Failed to open input file.");
		return 1;
	}

	std::string input{ std::istreambuf_iterator{ file }, {} };

	auto numericMoves{ Keypad::possible_numeric_moves() };
	auto directionalMoves{ Keypad::possible_directional_moves() };

	std::unordered_map<std::pair<std::string, int>, std::size_t, CacheHash> cache;

	std::size_t total{ 0 };

	for (const auto line : std::views::split(input, '\n'))
	{
		std::string_view code{ line };

		std::vector<std::vector<std::string>> movesets;

		for (const auto& [from, to] : std::views::zip("A" + std::string{ code }, line))
		{
			movesets.push_back(numericMoves[std::make_pair(from, to)]);
		}

		auto moves{ cartesian_product(movesets) };

		std::vector<std::size_t> lengths;

		for (const auto& move : moves)
		{
			lengths.push_back(path_length(directionalMoves, cache, move));
		}

		std::size_t number{};

		std::from_chars(code.data(), code.data() + code.size(), number);

		if (auto minIterator{ std::ranges::min_element(lengths) }; minIterator != lengths.end())
		{
			std::println("{} : {}", line, *minIterator);
			total += *minIterator * number;
		}
	}

	std::println("{}", total);
}