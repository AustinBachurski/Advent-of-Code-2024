module;

#include <deque>
#include <mdspan>
#include <ranges>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

export module Keypads;

export namespace Keypad
{
	export struct PairHash
	{
		std::size_t operator() (const std::pair<char, char> pair) const noexcept
		{
			std::hash<char> hash{ std::hash<char>{} };

			auto h1{ hash(pair.first) };
			auto h2{ hash(pair.second) };

			return h1 + (h2 << 1);
		}
	};

	struct Coordinate
	{
		int x{};
		int y{};
	};

	struct Move
	{
		Coordinate coord{};
		std::string move;
	};

	constexpr std::array<Move, 4> get_next_moves(const Coordinate& coord)
	{
		return { {
			{ { coord.x - 1, coord.y }, "<" },
			{ { coord.x + 1, coord.y }, ">" },
			{ { coord.x, coord.y - 1 }, "^" },
			{ { coord.x, coord.y + 1 }, "v" },
		} };
	}

	template <typename T>
	concept MdSpan = requires(T s)
	{
		{ s.extents() } -> std::convertible_to<typename T::extents_type>;
	};

	template <MdSpan T>
	constexpr bool valid_coordinate(const Coordinate& coord, const T keypad)
	{
		return coord.x >= 0 && coord.x < keypad.extent(1)
			&& coord.y >= 0 && coord.y < keypad.extent(0);
	}


	template <MdSpan T>
	std::unordered_map<std::pair<char, char>, std::vector<std::string>, PairHash> steps_for_all_moves(const T keypad)
	{
		std::unordered_map<char, Coordinate> positions;

		for (const auto y : std::views::iota(static_cast<unsigned long long>(0), keypad.extent(0)))
		{
			for (const auto x : std::views::iota(static_cast<unsigned long long>(0), keypad.extent(1)))
			{
				if (keypad[y, x] == '#')
				{
					continue;
				}

				positions[keypad[y, x]] = { static_cast<int>(x), static_cast<int>(y) };
			}
		}

		std::unordered_map<std::pair<char, char>, std::vector<std::string>, PairHash> possibleMoves;
		std::deque<std::pair<Coordinate, std::string>> queue;
		int bestCost{};

		for (const auto& [a, _] : positions)
		{
			for (const auto& [b, _] : positions)
			{
				if (a == b)
				{
					possibleMoves[std::make_pair(a, b)] = { "A" };
					continue;
				}

				queue.clear();
				queue.emplace_back(positions[a], "");
				bestCost = std::numeric_limits<int>::max();

				while (!queue.empty())
				{
					auto current{ queue.front() };
					queue.pop_front();

					for (const auto& move : get_next_moves(current.first))
					{
						if (!valid_coordinate(move.coord, keypad) || keypad[move.coord.y, move.coord.x] == '#')
						{
							continue;
						}

						if (keypad[move.coord.y, move.coord.x] == b)
						{
							if (bestCost < current.second.size())
							{
								queue.clear();
								break;
							}

							bestCost = static_cast<int>(current.second.size()) + 1;
							possibleMoves[std::make_pair(a, b)].push_back(current.second + move.move + 'A');
						}
						else
						{
							queue.emplace_back(move.coord, current.second + move.move);
						}
					}
				}
			}
		}

		return possibleMoves;
	}

	export std::unordered_map<std::pair<char, char>, std::vector<std::string>, PairHash> possible_numeric_moves()
	{
		constexpr std::string_view numericKeypadChars{
			"789456123#0A"
		};

		constexpr auto numericKeypad{ std::mdspan(numericKeypadChars.data(), 4, 3) };

		return steps_for_all_moves(numericKeypad);
	}

	export std::unordered_map<std::pair<char, char>, std::vector<std::string>, PairHash> possible_directional_moves()
	{
		constexpr std::string_view directionalKeypadChars{
			"#^A<v>"
		};

		constexpr auto directionalKeypad{ std::mdspan(directionalKeypadChars.data(), 2, 3) };

		return steps_for_all_moves(directionalKeypad);
	}
}

