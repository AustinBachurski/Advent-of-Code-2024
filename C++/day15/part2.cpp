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
	constexpr char bot      { '@' };
	constexpr char boxLeft  { '[' };
	constexpr char boxRight { ']' };
	constexpr char fitBox   { 'O' };
	constexpr char empty    { '.' };
	constexpr char wall     { '#' };
}

namespace mapDirection
{
	constexpr char up{ '^' };
	constexpr char down{ 'v' };
	constexpr char left{ '<' };
	constexpr char right{ '>' };
}

class XLWarehouseBot
{
public:
	XLWarehouseBot(std::string_view map)
	: m_map{ super_size_me(map) }
	{ 
		m_lineLength = m_map.find('\n') + 1;
		m_botPosition = m_map.find(mapItem::bot);
	}

	void move(const char direction)
	{
		switch (direction)
		{
		case mapDirection::up:
			[[fallthrough]];

		case mapDirection::down:
			return move_vertical(direction);

		case mapDirection::left:
			[[fallthrough]];

		case mapDirection::right:
			return move_horizontal(direction);
		}

		throw std::invalid_argument("Invalid direction: " + direction);
	}

	size_t sum_gps() const
	{
		size_t sum{};

		for (const auto [index, tile] : std::views::enumerate(m_map))
		{
			if (tile != mapItem::boxLeft)
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
	std::vector<size_t> get_indicies_horizontal(const char direction)
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

	std::string super_size_me(const std::string_view map) const
	{
		std::string xlMap;
		xlMap.reserve(map.size() * 2);

		for (const auto c : map)
		{
			switch (c)
			{
			case mapItem::wall:
				xlMap += "##";
				continue;

			case mapItem::fitBox:
				xlMap += "[]";
				continue;

			case mapItem::empty:
				xlMap += "..";
				continue;

			case mapItem::bot:
				xlMap += "@.";
				continue;

			case '\n':
				xlMap += '\n';
				continue;

			default:
				throw std::invalid_argument("Invalid map tile: " + c);
			}
		}

		return xlMap;
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

	bool is_valid_move(const char direction, const size_t index) const
	{
		auto next{ m_map.at(index + value_for(direction)) };

		if (next == mapItem::empty)
		{
			return true;
		}

		if (next == mapItem::wall)
		{
			return false;
		}

		if (next == mapItem::boxLeft)
		{
			return is_valid_move(direction, index + value_for(direction))
				&& is_valid_move(direction, index + value_for(direction) + 1);
		}
		else
		{
			return is_valid_move(direction, index + value_for(direction))
				&& is_valid_move(direction, index + value_for(direction) - 1);
		}
	}

	void move_boxes(const char direction, const size_t index)
	{
		auto target = index + value_for(direction);
		auto next{ m_map.at(target) };

		if (next == mapItem::boxLeft)
		{
			move_boxes(direction, index + value_for(direction));
			move_boxes(direction, index + value_for(direction) + 1);
		}
		else if (next == mapItem::boxRight)
		{
			move_boxes(direction, index + value_for(direction));
			move_boxes(direction, index + value_for(direction) - 1);
		}

		next = m_map.at(target);

		if (next == mapItem::empty)
		{
			m_map.at(target) = m_map.at(index);
			m_map.at(index) = next;

			if (index == m_botPosition)
			{
				m_botPosition = target;
			}
		}
	}

	void move_horizontal(const char direction)
	{ 
		std::vector<size_t> indicies{ get_indicies_horizontal(direction) };

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

	void move_vertical(const char direction)
	{ 
		if (!is_valid_move(direction, m_botPosition))
		{
			return;
		}

		move_boxes(direction, m_botPosition);
	}

	std::string m_map;
	size_t m_botPosition;
	size_t m_lineLength;
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

	XLWarehouseBot bot{ map };

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

