import KeyFinder;
import Keypad;

#include <fstream>
#include <iterator>
#include <print>
#include <ranges>


int main()
{
	std::ifstream file{ "C:/dev/cpp/Advent of Code/day21/input.txt", std::ios::in };

	if (!file.is_open())
	{
		std::println("Failed to open input file.");
		return 1;
	}

	std::string input{ std::istreambuf_iterator{ file }, {} };

	KeyFinder path;

	DirectionalKeypad controlKeypad1(path);
	DirectionalKeypad controlKeypad2(path, &controlKeypad1);
	NumericKeypad numericKeypad(path, &controlKeypad2);

	std::string command;
	std::vector<std::string> commands;
	std::size_t total{};

	int number{};

	for (const auto code : std::views::split(input, '\n'))
	{
		command.clear();
		number = 0;

		for (const char c : code)
		{
			command += numericKeypad.press(c);
		}

		std::println("Command: {}", command);

		commands.push_back(command);

		std::from_chars(code.data(), code.data() + code.size(), number);

		std::println("Number: {}", number);

		total += (commands.back().size() * number);
	}

	std::println("Total: {}", total);
}

