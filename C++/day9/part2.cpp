#include <algorithm>
#include <format>
#include <compare>
#include <numeric>
#include <fstream>
#include <iostream>
#include <print>
#include <ranges>
#include <set>
#include <string>
#include <string_view>
#include <vector>


class Block
{
public:
	Block() {};
	Block(size_t value)
		: value{ value }
		, m_valid{ true }
	{ }

	size_t value = 0;

	operator bool() const 
	{
		return m_valid;
	}

private:
	bool m_valid = false;
};

template<>
struct std::formatter<Block>
{
	constexpr auto parse(std::format_parse_context& context)
	{
		return context.begin();
	}

	auto format(const Block& block, std::format_context& context) const
	{
		return std::format_to(context.out(), "{}", block ? block.value : '.');
	}
};

template<>
struct std::formatter<std::vector<Block>>
{
	constexpr auto parse(std::format_parse_context& context)
	{
		return context.begin();
	}

	auto format(const std::vector<Block>& container, std::format_context& context) const
	{
		auto out = context.out();
		for (const auto [index, each] : std::views::enumerate(container))
		{
			if (each)
			{
				out = std::format_to(out, "{}", each.value);
			}
			else
			{
				out = std::format_to(out, ".");
			}
		}

		return out;
	}
};

size_t find_available_space(size_t target, size_t stop, const std::vector<Block>& blocks)
{
	size_t start = 0;
	size_t size = 0;

	for (const auto [index, block] : std::views::enumerate(blocks))
	{
		if (index >= stop)
		{
			return 0;
		}

		if (!block && !start)
		{
			start = index;
			size = 1;

			if (size == target)
			{
				return start;
			}

			continue;
		}

		if (!block)
		{
			++size;

			if (size == target)
			{
				return start;
			}

			continue;
		}

		start = size = 0;
	}

	return 0;
}

int main()
{
	std::ifstream file{ "C:/dev/cpp/Advent of Code/day9/input.txt", std::ios::in };

	if (!file.is_open())
	{
		std::println("Failed to open input file.");
		return 1;
	}

	std::string line;
	std::getline(file, line);

	std::vector<Block> blocks;
	blocks.reserve(line.size() * 5);

	// Populate
	size_t value = 0;
	for (auto [mode, c] : std::views::enumerate(line))
	{
		if (!(mode & 1))
		{
			for (auto _ : std::views::iota(0, static_cast<short>(c) - 48))
			{
				blocks.emplace_back(value);
			}
			++value;
		}
		else
		{
			for (auto _ : std::views::iota(0, static_cast<short>(c) - 48))
			{
				blocks.emplace_back();
			}
		}
	}

	// Swap
	bool value_set = false;
	size_t skip = 0;
	size_t start = 0;

	for (const auto [index, block] : std::views::enumerate(blocks) | std::views::drop(skip) | std::views::reverse)
	{
		if (block && !value_set)
		{
			start = index;
			value = block.value;
			value_set = true;
			++skip;
			continue;
		}

		if (block && block.value == value)
		{
			++skip;
			continue;
		}

		size_t size = start - index;
		size_t location = find_available_space(size, index + 2, blocks);

		if (location)
		{
			for (const auto _ : std::views::iota(static_cast<size_t>(0), size))
			{
				Block temp = blocks.at(start);
				blocks.at(start--) = blocks.at(location);
				blocks.at(location++) = temp;
			}
		}

		if (!block)
		{
			value_set = false;
		}
		else
		{
			start = index;
			value = block.value;
		}
	}

	size_t checksum = 0;

	for (const auto [index, block] : std::views::enumerate(blocks) | std::views::drop(1))
	{
		if (!block)
		{
			continue;
		}

		checksum += index * block.value;
	}

	std::println("{}", checksum);
}

