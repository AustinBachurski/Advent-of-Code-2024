module;

#include <print>
#include <stdexcept>
#include <span>
#include <string>
#include <vector>

export module Keypad;

import KeyFinder;

export class DirectionalKeypad
{
public:
	DirectionalKeypad(KeyFinder& path, DirectionalKeypad* master = nullptr)
		: m_path{ path }
		, m_master{ master }
	{ }

	[[nodiscard]]
	std::vector<char> move_and_press(const std::vector<char>& path)
	{
		std::vector<char> updatedPath;

		if (path.empty())
		{
			updatedPath.push_back('A');
		}

		for (const auto direction : path)
		{
			updatedPath.append_range(m_path.find_path(direction, m_position));
			updatedPath.push_back('A');
			m_position = m_path.key_location(direction, Keypad::directional);
		}

		if (m_master)
		{
			return m_master->move_and_press(updatedPath);
		}

		return updatedPath;
	}

private:
	KeyFinder& m_path;
	DirectionalKeypad* m_master{ nullptr };
	int m_position{ 8 };

};

export class NumericKeypad
{
public:
	NumericKeypad(KeyFinder& path, DirectionalKeypad* master)
		: m_path{ path }
		, m_master{ master }
	{
	}

	std::string press(const char c)
	{
		auto paths{ m_path.find_best_paths(c, m_position) };

		m_position = m_path.key_location(c, Keypad::numeric);

		if (!m_master)
		{
			return {};
		}

		std::vector<std::vector<char>> commandSets;

		for (const auto& path : paths)
		{
			commandSets.push_back(m_master->move_and_press(path));
		}

		auto shortest{ std::ranges::min(commandSets, [](const std::span<char> lhs, const std::span<char> rhs)
			{
				return lhs.size() < rhs.size();
			})};

		std::string commands;
		commands.reserve(shortest.size());

		for (const auto c : shortest)
		{
			commands += c;
		}

		return commands;
	}

private:
	KeyFinder& m_path;
	DirectionalKeypad* m_master{ nullptr };

	int m_position{ 23 };
};

