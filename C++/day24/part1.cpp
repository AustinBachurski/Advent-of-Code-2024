#include <algorithm>
#include <thread>
#include <chrono>
#include <deque>
#include <array>
#include <fstream>
#include <iostream>
#include <iterator>
#include <print>
#include <queue>
#include <ranges>
#include <stdexcept>
#include <unordered_set>
#include <map>
#include <vector>



template <typename T>
concept StringViewable = requires(T s)
{
	std::string_view{ *s.begin() };
};

template <StringViewable T>
struct OpCodes
{
	OpCodes(T view)
	{
		if (auto size{ std::distance(view.begin(), view.end()) }; size != 5)
		{
			throw std::invalid_argument("View size was incorrect: " + std::to_string(size));
		}

		auto it{ view.begin() };

		leftWire = std::string_view{ *it++ };
		instruction = std::string_view{ *it++ };
		rightWire = std::string_view{ *it++ };
		outputWire = std::string_view{ *++it };
	}

	std::string_view leftWire{};
	std::string_view instruction{};
	std::string_view rightWire{};
	std::string_view outputWire{};
};

template <StringViewable T>
void execute(const OpCodes<T>& opcodes, std::map<std::string_view, bool>& wires)
{
	using namespace std::string_view_literals;

	if (opcodes.instruction == "AND"sv)
	{
		wires[opcodes.outputWire] = wires[opcodes.leftWire] & wires[opcodes.rightWire];
		return;
	}

	if (opcodes.instruction == "XOR"sv)
	{
		wires[opcodes.outputWire] = wires[opcodes.leftWire] ^ wires[opcodes.rightWire];
		return;
	}

	if (opcodes.instruction == "OR"sv)
	{
		wires[opcodes.outputWire] = wires[opcodes.leftWire] | wires[opcodes.rightWire];
		return;
	}

	std::string error{ "Invalid instruction: " };
	error.append(opcodes.instruction);

	throw std::invalid_argument(error);
}

int main()
{
	std::ifstream file("./input.txt", std::ios::in);

	if (!file.is_open())
	{
		std::println("Failed to open input file.");
		return 1;
	}

	const std::string input(std::istreambuf_iterator<char>{file}, {});

	using namespace std::string_view_literals;
	auto sections{ std::views::split(input, "\n\n"sv) };
	auto setup{ std::views::split(*sections.begin(), '\n') };
	auto instructions{ std::views::split(*++sections.begin(), '\n') };

	std::map<std::string_view, bool> wires;
	std::unordered_set<std::string_view> active;

	for (const auto values : setup)
	{
		auto wireAndValue{ std::views::split(values, ": "sv) };
		std::string_view wire{ *wireAndValue.begin() };
		std::string_view value{ *++wireAndValue.begin() };

		wires[wire] = value == "1"sv ? true : false;
		active.insert(wire);
	}

	std::deque<OpCodes<decltype(instructions)>> queue;

	for (const auto instructionSet : instructions)
	{
		auto instruction{ std::views::split(instructionSet, ' ') };
		queue.emplace_back(instruction);
	}

	while (!queue.empty())
	{
		auto current{ queue.front() };
		queue.pop_front();

		if (!active.contains(current.leftWire) || !active.contains(current.rightWire))
		{
			queue.push_back(current);
			continue;
		}

		active.insert(current.outputWire);
		execute(current, wires);
	}

	std::string binary;
	binary.reserve(wires.size());

	for (const auto& [wire, value] : std::views::reverse(wires))
	{
		if (!wire.starts_with('z'))
		{
			break;
		}
		binary += (value ? '1' : '0');
	}

	std::println("{}", std::stoull(binary, nullptr, 2));
}

