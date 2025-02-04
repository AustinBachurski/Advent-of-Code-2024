#include <algorithm>
#include <execution>
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
	double x{};
	double y{};
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
		throw std::invalid_argument("First value not convertable to <double>: " + std::string{ line.substr(start, end) });
	}

	start = line.find('Y', start) + 2;

	if (!(std::from_chars(line.data() + start, line.data() + line.size(), coords.y).ec == std::errc{}))
	{
		throw std::invalid_argument("Second value not convertable to <double>: " + std::string{ line.substr(start, end) });
	}

	return coords;
}

Claw make_contraption(const std::string_view a, const std::string_view b, const std::string_view p)
{
	Claw claw {
		extract_coordinates(a),
		extract_coordinates(b),
		extract_coordinates(p)
	};

	constexpr double lol{ 10000000000000 };

	claw.prize.x += lol;
	claw.prize.y += lol;

	return claw;
}

uint64_t total_tokens(const uint64_t a, const uint64_t b)
{
	return a * 3 + b;
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
	uint64_t sum{};

	while (std::getline(file, a) && std::getline(file, b) && std::getline(file, p))
	{
		claw = make_contraption(a, b, p);

		auto A = (claw.prize.x * claw.b.y - claw.prize.y * claw.b.x) / (claw.a.x * claw.b.y - claw.a.y * claw.b.x);
		auto B = (claw.a.x * claw.prize.y - claw.a.y * claw.prize.x) / (claw.a.x * claw.b.y - claw.a.y * claw.b.x);

		auto A_int = static_cast<uint64_t>(A);
		auto B_int = static_cast<uint64_t>(B);
		
		if (static_cast<double>(A_int) == A && static_cast<double>(B_int) == B)
		{
			sum += total_tokens(A, B);
		}

		std::getline(file, a);
	}

	std::println("{}", sum);

}

