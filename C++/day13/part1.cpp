#include <algorithm>
#include <numeric>
#include <fstream>
#include <print>
#include <ranges>
#include <set>
#include <string>
#include <string_view>
#include <vector>


struct Coords
{
	int x{};
	int y{};
};

struct Claw
{
	Coords a;
	Coords b;
	Coords prize;
};

Coords extract_coordinates(const std::string_view line)
{
	Coords coords;

	size_t start = line.find('X') + 2;
	size_t end = line.find(',');

	if (!(std::from_chars(line.data() + start, line.data() + end, coords.x).ec == std::errc{}))
	{
		throw std::invalid_argument("First value not convertable to <int>: " + std::string{ line.substr(start, end) });
	}

	start = line.find('Y', start) + 2;

	if (!(std::from_chars(line.data() + start, line.data() + line.size(), coords.y).ec == std::errc{}))
	{
		throw std::invalid_argument("Second value not convertable to <int>: " + std::string{ line.substr(start, end) });
	}

	return coords;
}

Claw make_contraption(const std::string_view a, const std::string_view b, const std::string_view p)
{
	return {
		extract_coordinates(a),
		extract_coordinates(b),
		extract_coordinates(p)
	};
}

int total_tokens(const int a, const int b)
{
	return a * 3 + b;
}

int win_prizes(const Claw& claw)
{
	constexpr int max = 100;

	std::set<int> wins;

	for (const auto a_presses : std::views::iota(0, max + 1))
	{
		auto x = claw.a.x * a_presses;
		auto y = claw.a.y * a_presses;

		if (x > claw.prize.x || y > claw.prize.y)
		{
			break;
		}

		const int x_remaining = claw.prize.x - x;
		const int y_remaining = claw.prize.y - y;

		if (!(x_remaining % claw.b.x == 0 && y_remaining % claw.b.y == 0))
		{
			continue;
		}

		const int b_presses_x = x_remaining / claw.b.x;
		const int b_presses_y = y_remaining / claw.b.y;

		if (b_presses_x == b_presses_y)
		{
			wins.insert(total_tokens(a_presses, b_presses_x));
		}
	}

	return wins.size() ? *wins.begin() : 0;
}


int main()
{
	std::ifstream file{ "C:/dev/cpp/Advent of Code/day13/input.txt", std::ios::in };

	if (!file.is_open())
	{
		std::println("Failed to open input file.");
		return 1;
	}

	std::string a;
	std::string b;
	std::string p;

	Claw claw;
	int sum{};

	while (std::getline(file, a) && std::getline(file, b) && std::getline(file, p))
	{
		claw = make_contraption(a, b, p);

		sum += win_prizes(claw);

		std::getline(file, a);
	}

	std::println("{}", sum);

}

