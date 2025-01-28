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

class Hike
{
public:
	Hike(const std::string_view map)
		: m_lineLength{ map.find('\n') + 1 }
		, m_map { map }
	{}

	void map_trails()
	{
		for (const auto [index, c] : std::views::enumerate(m_map))
		{
			if (c == '0')
			{
				up(index, c);
				down(index, c);
				left(index, c);
				right(index, c);
			}
		}
		std::println("{}", m_count);
	}

private:
	void up(size_t index, const char previous)
	{ 
		size_t step = index - m_lineLength;

		char current{ step < m_map.size() ? m_map.at(step) : ' ' };

		if (current != (previous + 1))
		{
			return;
		}

		if (current == '9')
		{
			++m_count;
			return;
		}

		up(step, current);
		down(step, current);
		left(step, current);
		right(step, current);
	}

	void down(size_t index, const char previous)
	{ 
		size_t step = index + m_lineLength;

		char current{ step < m_map.size() ? m_map.at(step) : ' ' };

		if (current != (previous + 1))
		{
			return;
		}

		if (current == '9')
		{
			++m_count;
			return;
		}

		up(step, current);
		down(step, current);
		left(step, current);
		right(step, current);
	}

	void left(size_t index, const char previous)
	{ 
		size_t step = index - 1;

		char current{ step < m_map.size() ? m_map.at(step) : ' ' };

		if (current != (previous + 1))
		{
			return;
		}

		if (current == '9')
		{
			++m_count;
			return;
		}

		up(step, current);
		down(step, current);
		left(step, current);
		right(step, current);
	}

	void right(size_t index, const char previous)
	{ 
		size_t step = index + 1;

		char current{ step < m_map.size() ? m_map.at(step) : ' ' };

		if (current != (previous + 1))
		{
			return;
		}

		if (current == '9')
		{
			++m_count;
			return;
		}

		up(step, current);
		down(step, current);
		left(step, current);
		right(step, current);
	}

	size_t m_lineLength{};
	size_t m_count{};
	std::string_view m_map{};
};


int main()
{
	std::ifstream file{ "C:/dev/cpp/Advent of Code/day10/input.txt", std::ios::in };

	if (!file.is_open())
	{
		std::println("Failed to open input file.");
		return 1;
	}

	std::string map{ std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>() };

	Hike hike{ map };

	hike.map_trails();
}

