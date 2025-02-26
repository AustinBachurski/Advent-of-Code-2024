#include <algorithm>
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


class Computer
{
public:
	explicit Computer(std::ifstream& file) noexcept
	{
		set_registers(file);
		set_instructions(file);
	}

	void execute()
	{
		while (execute_instruction())
		{ }

		auto output{ m_output | std::views::join_with(',') };

		for (const auto each : output)
		{
			std::print("{}", each);
		}
	}

private:
	struct Instruction
	{
		int opcode{};
		int operand{};
	};

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

	static int get_register_value(const std::string_view text) noexcept
	{
		constexpr std::size_t registersBegin{ 12 };

		const std::string_view data{ text.substr(registersBegin) };

		int value{};
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

	void set_instructions(std::ifstream& file)
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

			std::from_chars(opcode.data(), opcode.data() + 1, instruction.opcode);
			std::from_chars(operand.data(), operand.data() + 1, instruction.operand);

			m_instructions.push_back(instruction);
		}
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

	int evaluate_combo(const int operand)
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

	void adv(const int operand, int& out)
	{
		auto numerator{ m_registerA };
		auto denomintor{ std::pow(2, evaluate_combo(operand)) };

		if (denomintor == 0)
		{
			// Assuming we shouldn't get here.
			throw std::logic_error("Attempted division by zero.");
		}

		out = numerator / denomintor;
		++m_IP;
	}

	void bxl(const int operand)
	{
		m_registerB ^= operand;
		++m_IP;
	}

	void bst(const int operand)
	{
		m_registerB = evaluate_combo(operand) % 8;
		++m_IP;
	}

	void jnz(const int operand)
	{
		if (0 == m_registerA)
		{
			++m_IP;
			return;
		}

		m_IP = operand;
	}

	void bxc(const int operand)
	{
		m_registerB ^= m_registerC;
		++m_IP;
	}

	void out(const int operand)
	{
		m_output.push_back(std::to_string(evaluate_combo(operand) % 8));
		++m_IP;
	}

	std::vector<Instruction> m_instructions;
	std::size_t m_IP{};
	std::vector<std::string> m_output;
	int m_registerA{};
	int m_registerB{};
	int m_registerC{};

};

int main()
{
	std::ifstream file{ "C:/dev/cpp/Advent of Code/day17/input.txt", std::ios::in };

	if (!file.is_open())
	{
		std::println("Failed to open input file.");
	}

	Computer computer{ file };

	computer.execute();
}

