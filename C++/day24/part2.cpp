#include <algorithm>
#include <thread>
#include <chrono>
#include <array>
#include <fstream>
#include <iostream>
#include <iterator>
#include <optional>
#include <print>
#include <queue>
#include <ranges>
#include <stdexcept>
#include <set>
#include <unordered_set>
#include <map>
#include <vector>


enum class Bit
{
	carryIn,
	internalSum,
	internalCarry,
	intermediateCarry,
};

enum class Instruction
{
	XOR,
	AND,
	OR,
};

static constexpr auto get_instruction(std::string_view const text) -> Instruction
{
	if (text.contains("XOR"))
	{
		return Instruction::XOR;
	}
	else if (text.contains("AND"))
	{
		return Instruction::AND;
	}
	else
	{
		return Instruction::OR;
	}
}

static auto map_possibilities(std::string_view const text, std::map<std::string_view, std::unordered_set<Bit>> &bits) -> void
{
	Instruction const instruction{ get_instruction(text) };
	std::string_view const left{ text.substr(0, 3) };
	std::string_view const right{ text.substr(text.size() - 10, 3)};

	if (left.starts_with('x') && right.starts_with('y'))
	{
		return;
	}

	if (instruction == Instruction::XOR)
	{
		bits[left].insert(Bit::carryIn);
		bits[left].insert(Bit::internalSum);
		bits[right].insert(Bit::carryIn);
		bits[right].insert(Bit::internalSum);
	}
	else if (instruction == Instruction::AND)
	{
		bits[left].insert(Bit::carryIn);
		bits[left].insert(Bit::internalSum);
		bits[right].insert(Bit::carryIn);
		bits[right].insert(Bit::internalSum);
	}
	else if (instruction == Instruction::OR)
	{
		bits[left].insert(Bit::intermediateCarry);
		bits[left].insert(Bit::internalCarry);
		bits[right].insert(Bit::intermediateCarry);
		bits[right].insert(Bit::internalCarry);
	}
}

template <typename T>
concept StringViewable = requires(T s)
{
	std::string_view{ *s.begin() };
};

template <StringViewable T>
static auto check_intermediate_usage(std::string_view const intermediate, T instructions, std::map<std::string_view, std::unordered_set<Bit>> &bits, std::set<std::string_view> &errors) -> void
{
	for (auto const instructionLine : instructions)
	{ 
		std::string_view const instructionText{ instructionLine };
		std::string_view const expression{ instructionText.substr(0, instructionText.size() - 7) };

		if (expression.contains(intermediate))
		{
			std::string_view const other{ expression.starts_with(intermediate) ? expression.substr(expression.size() - 3) : expression.substr(0, 3) };

			Instruction const instruction{ get_instruction(instructionText) };
			std::string_view const output{ instructionText.substr(instructionText.size() - 3) };

			if (instruction == Instruction::OR)
			{
				if (bits[output].contains(Bit::carryIn))
				{
					return;
				}
				errors.insert(output);
			}

			if (instruction == Instruction::XOR)
			{
				errors.insert(intermediate);
			}

			if (instruction == Instruction::AND)
			{
				errors.insert(intermediate);
			}
		}
	}
}

template <StringViewable T>
static auto check_internal_carry_usage(std::string_view const internalCarry, T instructions, std::map<std::string_view, std::unordered_set<Bit>> &bits, std::set<std::string_view> &errors) -> void
{
	for (auto const instructionLine : instructions)
	{ 
		std::string_view const instructionText{ instructionLine };
		std::string_view const expression{ instructionText.substr(0, instructionText.size() - 7) };

		if (expression.contains(internalCarry))
		{
			std::string_view const other{ expression.starts_with(internalCarry) ? expression.substr(expression.size() - 3) : expression.substr(0, 3) };

			Instruction const instruction{ get_instruction(instructionText) };
			std::string_view const output{ instructionText.substr(instructionText.size() - 3) };

			if (instruction == Instruction::OR)
			{
				if (bits[output].contains(Bit::carryIn))
				{
					return;
				}
				errors.insert(output);
			}

			if (instruction == Instruction::XOR)
			{
				errors.insert(internalCarry);
			}

			if (instruction == Instruction::AND)
			{
				errors.insert(internalCarry);
			}
		}
	}
}

template <StringViewable T>
static auto check_internal_sum_usage(std::string_view const internalSum, T instructions, std::map<std::string_view, std::unordered_set<Bit>> &bits, std::set<std::string_view> &errors) -> void
{
	for (auto const instructionLine : instructions)
	{ 
		std::string_view const instructionText{ instructionLine };
		std::string_view const expression{ instructionText.substr(0, instructionText.size() - 7) };

		if (expression.contains(internalSum))
		{
			std::string_view const other{ expression.starts_with(internalSum) ? expression.substr(expression.size() - 3) : expression.substr(0, 3) };

			Instruction const instruction{ get_instruction(instructionText) };
			std::string_view const output{ instructionText.substr(instructionText.size() - 3) };

			if (instruction == Instruction::XOR)
			{
				if (output.starts_with('z'))
				{
					continue;
				}
				errors.insert(output);
			}

			if (instruction == Instruction::AND)
			{
				if (bits[output].contains(Bit::intermediateCarry))
				{
					check_intermediate_usage(output, instructions, bits, errors);
					continue;
				}
				errors.insert(output);
			}
		}
	}
}

template <StringViewable T>
static auto check_for_errors(std::string_view const operand, T instructions, std::map<std::string_view, std::unordered_set<Bit>> &bits, std::set<std::string_view> &errors) -> void
{
	for (auto const instructionLine : instructions)
	{ 
		std::string_view const instructionText{ instructionLine };
		std::string_view const expression{ instructionText.substr(0, instructionText.size() - 7) };

		if (expression.contains(operand))
		{
			Instruction const instruction{ get_instruction(instructionText) };
			std::string_view const output{ instructionText.substr(instructionText.size() - 3) };

			if (instruction == Instruction::XOR)
			{
				if (bits[output].contains(Bit::internalSum))
				{
					check_internal_sum_usage(output, instructions, bits, errors);
					continue;
				}
				errors.insert(output);
			}

			if (instruction == Instruction::AND)
			{
				if (bits[output].contains(Bit::internalCarry))
				{
					check_internal_carry_usage(output, instructions, bits, errors);
					continue;
				}
				errors.insert(output);
			}
		}
	}
}

int main()
{
	std::ifstream file("C:/dev/cpp/Advent of Code/day24/input.txt", std::ios::in);

	if (!file.is_open())
	{
		std::println("Failed to open input file.");
		return 1;
	}

	std::string const input(std::istreambuf_iterator<char>{file}, {});

	using namespace std::string_view_literals;
	auto sections{ std::views::split(input, "\n\n"sv) };
	auto setup{ std::views::split(*sections.begin(), '\n') };
	auto instructions{ std::views::split(*++sections.begin(), '\n') };

	std::map<std::string_view, std::unordered_set<Bit>> bits;

	for (auto instruction : instructions)
	{
		map_possibilities(std::string_view{ instruction }, bits);
	}

	std::set<std::string_view> errors;
	auto xRegisters{ [](auto view) { return std::string_view{ view }.contains('x'); } };
	for (auto const operandValue : setup | std::views::filter(xRegisters)
										 | std::views::take(44)
										 | std::views::drop(1))
	{
		std::string_view const value{ operandValue };
		std::string_view const operand{ value.substr(0, 3) };

		check_for_errors(operand, instructions, bits, errors);
	}

	auto output{ std::views::join_with(errors, ',') };

	for (auto const &each : output)
	{
		std::print("{}", each);
	}
}

