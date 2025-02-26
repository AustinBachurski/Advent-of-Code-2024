#include <algorithm>
#include <execution>
#include <format>
#include <compare>
#include <numeric>
#include <fstream>
#include <iostream>
#include <print>
#include <ranges>
#include <set>
#include <span>
#include <string>
#include <string_view>
#include <vector>

struct Instruction
{
	std::size_t opcode{};
	std::size_t operand{};
};

struct Registers
{
	std::size_t a{};
	std::size_t b{};
	std::size_t c{};
};

class Computer
{
public:
	explicit Computer(const Registers& registers, const std::vector<Instruction>& instructions, const std::vector<std::string>& target) noexcept
		: m_instructions{ instructions }
		, m_registerA{ registers.a }
		, m_registerB{ registers.b }
		, m_registerC{ registers.c }
		, m_target{ target }
		, m_value{ registers.a }
	{
	}

	bool execute()
	{
		while (execute_instruction())
		{
		}

		// Find output length range.
		/*
		std::print("\t");

		for (const auto& pair : m_instructions)
		{
			std::print("{},{},", pair.opcode, pair.operand);
		}

		std::println();

		auto output{ m_output | std::views::join_with(',') };

		std::print("\t");
		for (const auto each : output)
		{
			std::print("{}", each);
		}
		std::println();
		std::println();
		*/

		// Narrow search window to matching digits.
		/*
		const auto i{ m_output.size() - 1 };
		if (m_output.back() == m_target.back()
			&& m_output.at(i - 1) == m_target.at(i - 1)
			&& m_output.at(i - 2) == m_target.at(i - 2)
			&& m_output.at(i - 3) == m_target.at(i - 3)
			&& m_output.at(i - 4) == m_target.at(i - 4)
			&& m_output.at(i - 5) == m_target.at(i - 5)
			&& m_output.at(i - 6) == m_target.at(i - 6)
			&& m_output.at(i - 7) == m_target.at(i - 7)
			&& m_output.at(i - 8) == m_target.at(i - 8)
			&& m_output.at(i - 9) == m_target.at(i - 9)
			&& m_output.at(i - 10) == m_target.at(i - 10)
			&& m_output.at(i - 11) == m_target.at(i - 11)
			&& m_output.at(i - 12) == m_target.at(i - 12)
			&& m_output.at(i - 13) == m_target.at(i - 13)
			&& m_output.at(i - 14) == m_target.at(i - 14)
			)
		{
			std::println("{}", m_value);
			std::print("\t");

			for (const auto& pair : m_instructions)
			{
				std::print("{},{},", pair.opcode, pair.operand);
			}

			std::println();

			auto output{ m_output | std::views::join_with(',') };

			std::print("\t");
			for (const auto each : output)
			{
				std::print("{}", each);
			}
			std::println();
			std::println();

		}
		*/

		if (m_target == m_output)
		{
			return true;
		}
		return false;
	}

private:

	enum class Opcode
	{
		adv,
		bxl,
		bst,
		jnz,
		bxc,
		out,
		bdv,
		cdv,
	};

	static std::size_t get_register_value(const std::string_view text) noexcept
	{
		constexpr std::size_t registersBegin{ 12 };

		const std::string_view data{ text.substr(registersBegin) };

		std::size_t value{};
		std::from_chars(data.data(), data.data() + data.size(), value);

		return value;
	}

	void set_registers(std::ifstream& file) noexcept
	{
		std::string input;

		std::getline(file, input);
		m_registerA = get_register_value(input);

		std::getline(file, input);
		m_registerB = get_register_value(input);

		std::getline(file, input);
		m_registerC = get_register_value(input);
	}

	void set_instructions_and_target(std::ifstream& file)
	{
		constexpr std::size_t instructionsBegin{ 9 };

		std::string input;
		std::getline(file, input);
		std::getline(file, input);
		std::string_view inputView{ input };

		const std::string_view text{ inputView.substr(instructionsBegin) };

		Instruction instruction;

		auto values{ std::views::split(text, ',') };
		for (const auto& pair : values | std::views::chunk(2))
		{
			std::string_view opcode{ *pair.begin() };
			std::string_view operand{ *(++pair.begin()) };
			m_target.emplace_back(opcode);
			m_target.emplace_back(operand);

			std::from_chars(opcode.data(), opcode.data() + 1, instruction.opcode);
			std::from_chars(operand.data(), operand.data() + 1, instruction.operand);

			m_instructions.push_back(instruction);
		}

		auto targetView{ std::views::filter(text, [](const char c) { return c == ','; }) };
	}

	bool execute_instruction()
	{
		if (m_IP >= m_instructions.size())
		{
			return false;
		}

		auto opcode{ m_instructions.at(m_IP).opcode };
		auto operand{ m_instructions.at(m_IP).operand };

		switch (static_cast<Opcode>(opcode))
		{
		case Opcode::adv:
			adv(operand, m_registerA);
			break;

		case Opcode::bxl:
			bxl(operand);
			break;

		case Opcode::bst:
			bst(operand);
			break;

		case Opcode::jnz:
			jnz(operand);
			break;

		case Opcode::bxc:
			bxc(operand);
			break;

		case Opcode::out:
			out(operand);
			break;

		case Opcode::bdv:
			adv(operand, m_registerB);
			break;

		case Opcode::cdv:
			adv(operand, m_registerC);
			break;
		}

		return true;
	}

	std::size_t evaluate_combo(const std::size_t operand)
	{
		switch (operand)
		{
		case 0:
			[[fallthrough]];

		case 1:
			[[fallthrough]];

		case 2:
			[[fallthrough]];

		case 3:
			return operand;

		case 4:
			return m_registerA;

		case 5:
			return m_registerB;

		case 6:
			return m_registerC;

		case 7:
			throw std::invalid_argument("Combo operand 7 is reserved and will not appear in valid programs.");
		}

		throw std::invalid_argument(std::to_string(operand) + " is not a valid operand!");
	}

	void adv(const std::size_t operand, std::size_t& out)
	{
		auto numerator{ m_registerA };
		auto denominator{ std::pow(2, evaluate_combo(operand)) };

		if (denominator == 0)
		{
			// Assuming we shouldn't get here.
			throw std::logic_error("Attempted division by zero.");
		}

		out = numerator / denominator;
		++m_IP;
	}

	void bxl(const std::size_t operand)
	{
		m_registerB ^= operand;
		++m_IP;
	}

	void bst(const std::size_t operand)
	{
		m_registerB = evaluate_combo(operand) % 8;
		++m_IP;
	}

	void jnz(const std::size_t operand)
	{
		if (0 == m_registerA)
		{
			++m_IP;
			return;
		}

		m_IP = operand;
	}

	void bxc(const std::size_t operand)
	{
		m_registerB ^= m_registerC;
		++m_IP;
	}

	void out(const std::size_t operand)
	{
		m_output.push_back(std::to_string(evaluate_combo(operand) % 8));
		++m_IP;
	}

	std::vector<Instruction> m_instructions;
	std::size_t m_IP{};
	std::vector<std::string> m_output;
	std::size_t m_registerA{};
	std::size_t m_registerB{};
	std::size_t m_registerC{};
	std::vector<std::string> m_target{};
	std::size_t m_value{};

};

static std::size_t get_register_value(const std::string_view text) noexcept
{
	constexpr std::size_t registersBegin{ 12 };

	const std::string_view data{ text.substr(registersBegin) };

	std::size_t value{};
	std::from_chars(data.data(), data.data() + data.size(), value);

	return value;
}

Registers get_registers(std::ifstream& file) noexcept
{
	Registers registers;
	std::string input;

	std::getline(file, input);
	registers.a = get_register_value(input);

	std::getline(file, input);
	registers.b = get_register_value(input);

	std::getline(file, input);
	registers.c = get_register_value(input);

	return registers;
}

std::vector<Instruction> get_instructions(std::ifstream& file)
{
	constexpr std::size_t instructionsBegin{ 9 };

	std::vector<Instruction> instructions;

	std::string input;
	std::getline(file, input);
	std::getline(file, input);
	std::string_view inputView{ input };

	const std::string_view text{ inputView.substr(instructionsBegin) };

	Instruction instruction;

	auto values{ std::views::split(text, ',') };
	for (const auto& pair : values | std::views::chunk(2))
	{
		std::string_view opcode{ *pair.begin() };
		std::string_view operand{ *(++pair.begin()) };

		std::from_chars(opcode.data(), opcode.data() + 1, instruction.opcode);
		std::from_chars(operand.data(), operand.data() + 1, instruction.operand);

		instructions.push_back(instruction);
	}

	auto targetView{ std::views::filter(text, [](const char c) { return c == ','; }) };
	return instructions;
}

int main()
{
	std::ifstream file{ "C:/dev/cpp/Advent of Code/day17/input.txt", std::ios::in };

	if (!file.is_open())
	{
		std::println("Failed to open input file.");
	}

	const Registers registers = get_registers(file);
	const std::vector<Instruction> instructions{ get_instructions(file) };
	std::vector<std::string> target;

	for (const auto& each : instructions)
	{
		target.push_back(std::to_string(each.opcode));
		target.push_back(std::to_string(each.operand));
	}

	std::mutex resultMutex;
	std::size_t result{};

	const std::size_t step{            100'000'000 };
	std::size_t start{          37'190'000'000'000 };

	// Minimum for Output Size: 35'184'372'088'832
	// Output Size Exceeded:   282'184'372'088'832 
	// Most digits match:       37'222'273'957'364
	std::size_t end{ start + step };

	// Narrowing Results
	/*
	for (const auto _ : std::views::iota(0, 100'000'000))
	{
		Computer computer{ { start, registers.b, registers.c }, instructions, target };
		if (computer.execute())
		{
			std::lock_guard _{ resultMutex };

			if (!result)
			{
				result = start;
			}
		}

		start += step;
	}
	*/

	// Brute force from mostly matching output.
	while (!result && start < 37'222'278'756'852)
	{
		std::println("{}..{}", start, end);
		auto range{ std::views::iota(start, end) };

		std::for_each(std::execution::par, range.begin(), range.end(), [&registers, &instructions, &target, &result, &resultMutex](const std::size_t i) {
			Computer computer{ { i, registers.b, registers.c }, instructions, target };
			if (computer.execute())
			{
				std::lock_guard _{ resultMutex };

				if (!result)
				{
					result = i;
				}
			}
			});

		start = end;

		if (std::numeric_limits<std::size_t>::max() - end < step)
		{
			end = std::numeric_limits<std::size_t>::max();
		}
		else
		{
			end += step;
		}
	}

	if (result)
	{
		std::println("{}", result);
	}
	else
	{
		std::puts("\nInput exhausted...");
	}
}

