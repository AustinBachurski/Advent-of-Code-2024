#include <algorithm>
#include <numeric>
#include <format>
#include <fstream>
#include <iostream>
#include <mdspan>
#include <print>
#include <ranges>
#include <set>
#include <string>
#include <string_view>
#include <vector>


namespace map
{
	constexpr int width{ 101 };
	constexpr int height{ 103 };
	constexpr int size{ width * height };
	constexpr int quadrant_size{ (width / 2) * (height / 2) };


	constexpr std::array<size_t, quadrant_size> fill_quadrant_one()
	{
		std::array<size_t, quadrant_size> array;

		for (const auto row : std::views::iota(0, height / 2))
		{
			for (const auto idx : std::views::iota(0, width / 2))
			{
				int index{ row * (width / 2) + static_cast<int>(idx) };
				int value{ row * width + static_cast<int>(idx) };
				array.at(index) = static_cast<int>(value);
			}
		}

		return array;
	}

	constexpr std::array<size_t, quadrant_size> fill_quadrant_two()
	{
		std::array<size_t, quadrant_size> array;

		for (const auto row : std::views::iota(0, height / 2))
		{
			for (const auto idx : std::views::iota(0, width / 2))
			{
				int index{ row * (width / 2) + static_cast<int>(idx) };
				int value{ row * width + static_cast<int>(idx) + (width / 2 + 1) };
				array.at(index) = static_cast<int>(value);
			}
		}

		return array;
	}

	constexpr std::array<size_t, quadrant_size> fill_quadrant_three()
	{
		std::array<size_t, quadrant_size> array;

		for (const auto row : std::views::iota(0, height / 2))
		{
			for (const auto idx : std::views::iota(0, width / 2))
			{
				int index{ row * (width / 2) + static_cast<int>(idx) };
				int value{ row * width + static_cast<int>(idx) + (width * (height / 2 + 1)) };
				array.at(index) = static_cast<int>(value);
			}
		}

		return array;
	}

	constexpr std::array<size_t, quadrant_size> fill_quadrant_four()
	{
		std::array<size_t, quadrant_size> array;

		for (const auto row : std::views::iota(0, height / 2))
		{
			for (const auto idx : std::views::iota(0, width / 2))
			{
				int index{ row * (width / 2) + static_cast<int>(idx) };
				int value{ row * width + static_cast<int>(idx) + (width * (height / 2 + 1)) + (width / 2 + 1) };
				array.at(index) = static_cast<int>(value);
			}
		}

		return array;
	}

	constexpr std::array<size_t, quadrant_size> quadrant_one{ fill_quadrant_one() };
	constexpr std::array<size_t, quadrant_size> quadrant_two{ fill_quadrant_two() };
	constexpr std::array<size_t, quadrant_size> quadrant_three{ fill_quadrant_three() };
	constexpr std::array<size_t, quadrant_size> quadrant_four{ fill_quadrant_four() };
}

struct Coords
{
	int x{};
	int y{};
};

class Robot
{
public:
	Robot(Coords position, Coords step)
		: pos{ position }
		, step{ step }
	{ }


	size_t get_index() const
	{
		return static_cast<size_t>(pos.x + pos.y * map::width);
	}

	auto operator++()
	{
		pos.x += step.x;
		pos.y += step.y;

		if (pos.x >= map::width)
		{
			pos.x -= map::width;
		}
		else if (pos.x < 0)
		{
			pos.x += map::width;
		}

		if (pos.y >= map::height)
		{
			pos.y -= map::height;
		}
		else if (pos.y < 0)
		{
			pos.y += map::height;
		}
	}

private:
	Coords pos;
	Coords step;
};

Robot make_robot(const std::string_view line)
{
	Coords position;
	Coords step;

	size_t start = line.find("p=") + 2;
	size_t end = line.find(',');

	if (!(std::from_chars(line.data() + start, line.data() + end, position.x).ec == std::errc{}))
	{
		throw std::invalid_argument("Failed to convert number: " + std::string{ line.substr(start, end) });
	}

	start = end + 1;
	end = line.find(' ', start);

	if (!(std::from_chars(line.data() + start, line.data() + end, position.y).ec == std::errc{}))
	{
		throw std::invalid_argument("Failed to convert number: " + std::string{ line.substr(start, end) });
	}

	start = line.find("v=") + 2;
	end = line.find(',', start);

	if (!(std::from_chars(line.data() + start, line.data() + end, step.x).ec == std::errc{}))
	{
		throw std::invalid_argument("Failed to convert number: " + std::string{ line.substr(start, end) });
	}

	start = end + 1;
	end = line.size();

	if (!(std::from_chars(line.data() + start, line.data() + end, step.y).ec == std::errc{}))
	{
		throw std::invalid_argument("Failed to convert number: " + std::string{ line.substr(start, end) });
	}

	return { position, step };
}

int main()
{
	std::ifstream file{ "C:/dev/cpp/Advent of Code/day14/input.txt", std::ios::in };

	if (!file.is_open())
	{
		std::println("Failed to open input file.");
		return 1;
	}

	std::string line;

	std::array<size_t, map::size> space{ };

	while (std::getline(file, line))
	{
		if (!line.size())
		{
			break;
		}

		Robot bot = make_robot(line);

		for (const auto i : std::views::iota(0, 100))
		{
			++bot;
		}
		++space.at(bot.get_index());
	}
	
	size_t one{};
	size_t two{};
	size_t three{};
	size_t four{};

	for (const auto [index, value] : std::views::enumerate(space))
	{
		if (std::ranges::contains(map::quadrant_one, index))
		{
			one += value;
		}
		else if (std::ranges::contains(map::quadrant_two, index))
		{
			two += value;
		}
		else if (std::ranges::contains(map::quadrant_three, index))
		{
			three += value;
		}
		else if (std::ranges::contains(map::quadrant_four, index))
		{
			four += value;
		}
	}

	size_t sum{ one * two * three * four };

	std::println("{}", sum);

}

