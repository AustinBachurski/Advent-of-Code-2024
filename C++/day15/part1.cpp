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

namespace mapItem
{
	constexpr char bot   { '@' };
	constexpr char box   { 'O' };
	constexpr char empty { '.' };
	constexpr char wall  { '#' };
}

namespace mapDirection
{
	constexpr char up{ '^' };
	constexpr char down{ 'v' };
	constexpr char left{ '<' };
	constexpr char right{ '>' };
}

class WarehouseBot
{
public:
	WarehouseBot(std::string_view map)
	: m_botPosition{ map.find(mapItem::bot) }
	, m_lineLength{ map.find('\n') + 1 }
	, m_map{ map }
	{ }

	void move(const char direction)
	{ 
		std::vector<size_t> indicies{ get_indicies(direction) };

		if (indicies.empty())
		{
			return;
		}

		char previous{ m_map.at(indicies.front()) };

		m_map.at(m_botPosition) = mapItem::empty;
		m_map.at(indicies.front()) = mapItem::bot;
		m_botPosition = indicies.front();

		for (const auto index : indicies | std::views::drop(1))
		{
			char swap{ m_map.at(index) };
			m_map.at(index) = previous;
			previous = swap;
		}
	}

	size_t sum_gps() const
	{
		size_t sum{};

		for (const auto [index, tile] : std::views::enumerate(m_map))
		{
			if (tile != mapItem::box)
			{
				continue;
			}

			size_t fromTop = index / m_lineLength;
			size_t fromLeft = index % m_lineLength;

			sum += 100 * fromTop + fromLeft;
		}

		return sum;
	}

	void print_map() const
	{
		std::println("{}", m_map);
	}

private:
	std::vector<size_t> get_indicies(const char direction)
	{
		std::vector<size_t> indicies;

		size_t index{ m_botPosition + value_for(direction) };

		while (m_map.at(index) != mapItem::wall)
		{
			indicies.push_back(index);

			if (m_map.at(index) == mapItem::empty)
			{
				return indicies;
			}

			index += value_for(direction);
		}

		indicies.clear();
		return indicies;
	}

	int value_for(const char direction) const
	{
		switch (direction)
		{
		case mapDirection::up:
			return -static_cast<int>(m_lineLength);

		case mapDirection::down:
			return static_cast<int>(m_lineLength);

		case mapDirection::left:
			return -1;

		case mapDirection::right:
			return 1;
		}

		throw std::invalid_argument("Index is not a direction: " + direction);
	}

	size_t m_botPosition;
	size_t m_lineLength;
	std::string m_map;
};

int main()
{
	std::ifstream file{ "C:/dev/cpp/Advent of Code/day15/input.txt", std::ios::in };

	if (!file.is_open())
	{
		std::println("Failed to open input file.");
		return 1;
	}

	std::string input{ std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>() };

	using namespace std::string_view_literals;
	auto split = std::views::split(input, "\n\n"sv);

	std::string_view map{ split.front() };
	std::string_view moves{ *(++split.begin()) };

	WarehouseBot bot{ map };

	for (const auto direction : moves)
	{
		if (direction == '\n')
		{
			continue;
		}

		bot.move(direction);
	}

	std::println("{}", bot.sum_gps());

}

