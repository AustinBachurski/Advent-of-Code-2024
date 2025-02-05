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
#include <span>
#include <thread>
#include <vector>


namespace map
{
	constexpr int width{ 101 };
	constexpr int height{ 103 };
	constexpr int size{ width * height };
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

void print_map(const std::span<char> map)
{

	for (const auto row : std::views::iota(0, map::height))
	{
		for (const auto column : std::views::iota(0, map::width))
		{
			std::print("{}", map[row * map::width + column]);
		}
		std::println();
	}
	std::println();
	std::println();
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

	std::vector<Robot> robots;

	while (std::getline(file, line))
	{
		if (!line.size())
		{
			break;
		}

		robots.push_back(make_robot(line));
	}

	size_t pattern = 47;

	for (const auto s : std::views::iota(0, 7623))
	{
		std::array<char, map::size> map{};
		map.fill(' ');

		for (auto& bot : robots)
		{
			++bot;
			map.at(bot.get_index()) = '#';
		}

		if (s == pattern)
		{
			if (s > 7600)
			{
				system("cls");
				std::println("Seconds: {}", s + 1);
				print_map(map);
				//std::this_thread::sleep_for(std::chrono::milliseconds(500));
			}
			pattern += 101;
		}
	}

}

