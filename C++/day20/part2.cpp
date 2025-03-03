#include <algorithm>
#include <thread>
#include <chrono>
#include <array>
#include <fstream>
#include <iostream>
#include <iterator>
#include <print>
#include <queue>
#include <ranges>
#include <set>
#include <unordered_set>
#include <unordered_map>
#include <map>
#include <vector>


class CheatEngine
{
public:
	CheatEngine(const std::string_view map)
		: m_map{ map }
	{
		m_startIndex = map.find(m_startTile);
		m_endIndex = map.find(m_endTile);
		m_lineLength = map.find('\n') + 1;
	}

	std::size_t count_cheats(const std::size_t maxCheatSteps, const std::size_t minimumTimeSaved)
	{
		m_maxCheatSteps = maxCheatSteps;
		m_minTimeSaved = minimumTimeSaved;

		m_path = get_path_values();

		return get_speedy_cheats();
	}

private:
	constexpr std::array<std::size_t, 4> get_next_moves(const std::size_t index)
	{
		const auto north{ index - m_lineLength };
		const auto south{ index + m_lineLength };
		const auto east{ index + 1 };
		const auto west{ index - 1 };

		return { north, south, east, west };
	}

	std::size_t distance(const std::size_t begin, const std::size_t end)
	{
		const auto beginX{ static_cast<long long>(begin % m_lineLength) };
		const auto beginY{ static_cast<long long>(begin / m_lineLength) };
		const auto endX{   static_cast<long long>(end % m_lineLength) };
		const auto endY{   static_cast<long long>(end / m_lineLength) };

		return std::abs(beginX - endX) + std::abs(beginY - endY);
	}

	std::size_t count_cheats(const std::size_t index)
	{
		return std::ranges::count_if(m_path, [&index, this](const std::pair<size_t, size_t>& pathTile)
			{
				auto steps{ distance(index, pathTile.first) };

				if (pathTile.second > m_path[index])
				{
					return steps <= m_maxCheatSteps && (pathTile.second - m_path[index] - steps) >= m_minTimeSaved;
				}
				return false;
			});
	}

	std::size_t get_speedy_cheats()
	{
		const std::size_t pointless{ m_endIndex - m_maxCheatSteps };

		std::size_t count{};

		for (const auto& [index, cost] : m_path)
		{
			if (cost >= pointless)
			{
				continue;
			}

			count += count_cheats(index);
		}

		return count;
	}

	std::unordered_map<std::size_t, std::size_t> get_path_values()
	{
		const auto end{ m_map.find(m_endTile) };

		std::unordered_map<std::size_t, std::size_t> path;
		std::size_t next{ m_map.find(m_startTile) };
		std::size_t previous{};
		std::size_t value{ 0 };

		while (!path.contains(next))
		{
			path[next] = value++;

			for (const auto index : get_next_moves(next))
			{
				if (index == previous || m_map[index] == m_wallTile)
				{
					continue;
				}

				previous = next;
				next = index;
				break;
			}
		}

		return path;
	}

	std::string_view m_map;
	std::unordered_map<std::size_t, std::size_t> m_path;

	std::size_t m_startIndex{};
	std::size_t m_endIndex{};
	std::size_t m_lineLength{};
	std::size_t m_maxCheatSteps{};
	std::size_t m_minTimeSaved{};

	char m_startTile{ 'S' };
	char m_endTile{ 'E' };
	char m_wallTile{ '#' };
	char m_emptyTile{ '.' };

};

int main()
{
	std::ifstream file("C:/dev/cpp/Advent of Code/day20/input.txt", std::ios::in);

	if (!file.is_open())
	{
		std::println("Failed to open input file.");
		return 1;
	}

	const std::string map(std::istreambuf_iterator<char>{file}, {});

	CheatEngine cheat(map);

	auto cheats{ cheat.count_cheats(20, 100) };

	std::println("{}", cheats);
}

