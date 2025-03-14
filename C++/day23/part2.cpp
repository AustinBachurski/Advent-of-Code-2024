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
#include <stack>
#include <unordered_set>
#include <unordered_map>
#include <vector>


int main()
{
	std::ifstream file("C:/dev/cpp/Advent of Code/day23/input.txt", std::ios::in);

	if (!file.is_open())
	{
		std::println("Failed to open input file.");
		return 1;
	}

	const std::string input(std::istreambuf_iterator<char>{file}, {});

	std::unordered_map<std::string_view, std::unordered_set<std::string_view>> groups;

	auto links{ std::views::split(input, '\n') };

	for (const auto link : links)
	{
		std::string_view pair{ link };
		std::string_view left{ pair.substr(0, 2) };
		std::string_view right{ pair.substr(3) };

		groups[left].insert(right);
		groups[right].insert(left);
	}

	std::unordered_map<std::string_view, std::set<std::string_view>> clusters;

	std::stack<std::string_view> toBeChecked;
	std::unordered_set<std::string_view> checked;
	std::string_view checking{};

	for (const auto& [currentID, connections] : groups)
	{
		clusters[currentID].insert(currentID);

		for (const auto connection : connections)
		{
			toBeChecked.push(connection);
		}

		while (!toBeChecked.empty())
		{
			checking = toBeChecked.top();
			toBeChecked.pop();

			auto allConnected{ std::ranges::all_of(clusters[currentID],
				[&groups, &checking](const std::string_view cluster)
				{
					return groups[checking].contains(cluster);
				}) };

			if (allConnected)
			{
				clusters[currentID].insert(checking);
			}
		}
	}

	auto max{ std::ranges::max_element(clusters, [](
		const std::pair<std::string_view, std::set<std::string_view>>& lhs,
		const std::pair<std::string_view, std::set<std::string_view>>& rhs)
		{
			return lhs.second.size() < rhs.second.size();
		}) };

	if (max != clusters.end())
	{
		std::string result;
		result.reserve(max->second.size() * 3);

		for (const auto each : max->second)
		{
			result += std::string{ each } + ',';
		}

		result.pop_back();
		std::println("{}", result);
	}
}

