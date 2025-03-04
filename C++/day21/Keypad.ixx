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
	std::vector<char> move_and_press(const std::span<char> path)
	{
		std::vector<char> updatedPath;

		if (path.empty())
		{
			updatedPath.push_back('A');
		}

		for (const auto direction : path)
		{
			updatedPath.append_range(m_path.find_path(direction, m_position, Keypad::directional));
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
		auto path{ m_path.find_path(c, m_position, Keypad::numeric) };
		path.push_back('A');
		m_position = m_path.key_location(c, Keypad::numeric);

		if (!m_master)
		{
			return {};
		}

		auto completePath = m_master->move_and_press(path);

		std::string commands;
		commands.reserve(completePath.size());

		for (const auto c : completePath)
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

