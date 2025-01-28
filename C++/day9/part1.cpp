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

	size_t source = blocks.size() - 1;
	Block temp;

	for (auto [target, block] : std::views::enumerate(blocks))
	{
		if (target >= source)
		{
			break;
		}

		if (!block)
		{
			temp = blocks.at(target);
			blocks.at(target) = blocks.at(source);
			blocks.at(source) = temp;
			--source;

			while (!blocks.at(source))
			{
				--source;
			}
		}
	}

	size_t checksum = 0;
	size_t index = 1;

	for (const auto& block : blocks | std::views::drop(1))
	{
		size_t check = checksum;
		if (!block)
		{
			break;
		}

		checksum += index * block.value;

		++index;
	}


	std::println("{}", checksum);
}
