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


enum class Direction
{
	up,
	down,
	left,
	right,
	loop_all,
};

struct Fence
{
	uint64_t area{};
	std::vector<std::pair<size_t, Direction>> perimiter{};

	void operator+= (const Fence& rhs)
	{
		area += rhs.area;
		perimiter.append_range(rhs.perimiter);
	}
};

class Garden
{
public:
	Garden(const std::string_view plots)
		: m_lineLength{ plots.find('\n') + 1 }
		, m_plots { plots }
	{}

	void calculate_region_cost()
	{
		uint64_t total{};
		for (const auto [index, c] : std::views::enumerate(m_plots))
		{
			if (c == '\n') { continue; }

			if (!m_visited.contains(index))
			{
				Fence required = explore_region(index, c);
				auto sides = count_sides(required.perimiter);
				auto value = required.area * sides;
				total += value;
			}
		}
		std::println("{}", total);
	}

private:
	Fence explore_region(const size_t index, const char c)
	{
		Fence required{ 1, {} };
		m_visited.insert(index);

		required += up(index, c);
		required += down(index, c);
		required += left(index, c);
		required += right(index, c);

		return required;
	}

	Fence up(size_t index, const char region)
	{ 
		size_t plot = index - m_lineLength;

		char current{ plot < m_plots.size() ? m_plots.at(plot) : ' ' };

		Fence required;

		if (current != region)
		{
			required.perimiter.emplace_back(plot, Direction::up);
			return required;
		}

		if (m_visited.contains(plot))
		{
			return required;
		}
		++required.area;
		m_visited.insert(plot);

		required += up(plot, region);
		required += left(plot, region);
		required += right(plot, region);

		return required;
	}

	Fence down(size_t index, const char region)
	{ 
		size_t plot = index + m_lineLength;

		char current{ plot < m_plots.size() ? m_plots.at(plot) : ' ' };

		Fence required;

		if (current != region)
		{
			required.perimiter.emplace_back(plot, Direction::down);
			return required;
		}

		if (m_visited.contains(plot))
		{
			return required;
		}
		++required.area;
		m_visited.insert(plot);

		required += down(plot, region);
		required += left(plot, region);
		required += right(plot, region);

		return required;
	}

	Fence left(size_t index, const char region)
	{ 
		size_t plot = index - 1;

		char current{ plot < m_plots.size() ? m_plots.at(plot) : ' ' };

		Fence required;

		if (current != region)
		{
			required.perimiter.emplace_back(plot, Direction::left);
			return required;
		}

		if (m_visited.contains(plot))
		{
			return required;
		}
		++required.area;
		m_visited.insert(plot);

		required += up(plot, region);
		required += down(plot, region);
		required += left(plot, region);

		return required;
	}

	Fence right(size_t index, const char region)
	{ 
		size_t plot = index + 1;

		char current{ plot < m_plots.size() ? m_plots.at(plot) : ' ' };

		Fence required;

		if (current != region)
		{
			required.perimiter.emplace_back(plot, Direction::right);
			return required;
		}

		if (m_visited.contains(plot))
		{
			return required;
		}
		++required.area;
		m_visited.insert(plot);

		required += up(plot, region);
		required += down(plot, region);
		required += right(plot, region);

		return required;
	}

	uint64_t count_sides(const std::vector<std::pair<size_t, Direction>>& fences)
	{
		std::set<size_t> candidates;

		uint64_t sides{};

		for (const int direction : std::views::iota(0, static_cast<int>(Direction::loop_all)))
		{
			candidates.clear();
			for (const auto& segment : fences)
			{
				if (segment.second == static_cast<Direction>(direction))
				{
					candidates.insert(segment.first);
				}
			}

			if (candidates.size() == 1)
			{
				++sides;
				continue;
			}

			auto remaining = candidates;

			for (const auto segment : candidates)
			{
				auto result = segment + stride_for(static_cast<Direction>(direction));

				while (remaining.contains(result))
				{
					remaining.erase(result);
					result += stride_for(static_cast<Direction>(direction));
				}
			}
			sides += remaining.size();
		}

		return sides;
	}

	constexpr int64_t stride_for(Direction direction) const
	{
		switch (direction)
		{
		case Direction::up:
			[[fallthrough]];

		case Direction::down:
			return 1;

		case Direction::left:
			[[fallthrough]];

		case Direction::right:
			return m_lineLength;
		}
		std::unreachable();
	}

	size_t m_lineLength{};
	std::string_view m_plots{};
	std::set<size_t> m_visited;
};


int main()
{
	std::ifstream file{ "C:/dev/cpp/Advent of Code/day12/input.txt", std::ios::in };

	if (!file.is_open())
	{
		std::println("Failed to open input file.");
		return 1;
	}

	std::string plots{ std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>() };

	Garden garden{ plots };

	garden.calculate_region_cost();
}

