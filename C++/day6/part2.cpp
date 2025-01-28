#include <algorithm>
#include <chrono>
#include <execution>
#include <fstream>
#include <numeric>
#include <print>
#include <ranges>
#include <string>
#include <string_view>
#include <vector>

class Timer
{
	std::chrono::time_point<std::chrono::steady_clock> start;

public:
	Timer()
	{
		start = std::chrono::steady_clock::now();
	}

	~Timer()
	{
		std::println("Elapsed Time: {}", std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start));
	}
};

class Pathing
{
public:
	Pathing(const size_t guard, const size_t line_length)
		: m_pos{ static_cast<int64_t>(guard) }
		, m_lineLength{ static_cast<int64_t>(line_length) }
	{}

	void turn()
	{
		m_pos -= m_move;

		switch (m_direction)
		{
		case Direction::up:
		{
			m_move = 1;
			m_direction = Direction::right;
			break;
		}

		case Direction::right:
		{
			m_move = m_lineLength;
			m_direction = Direction::down;
			break;
		}

		case Direction::down:
		{
			m_move = -1;
			m_direction = Direction::left;
			break;
		}

		case Direction::left:
		{
			m_move = -m_lineLength;
			m_direction = Direction::up;
			break;
		}
		}
	}

	void next()
	{
		m_pos += m_move;
	}

	int64_t m_pos;

private:
	enum class Direction
	{
		up,
		right,
		down,
		left,
	};

	Direction m_direction{ Direction::up };
	int64_t m_lineLength;
	int64_t m_move = { -m_lineLength };
};



int main()
{
	std::ifstream file{ "C:/dev/cpp/Advent of Code/day6/input.txt", std::ios::in };

	if (!file.is_open())
	{
		std::println("Failed to open input file.");
		return 1;
	}

	std::string input{ std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>() };

	size_t guard = input.find('^');
	size_t line_length = input.find('\n') + 1;
	
	Pathing path{ guard, line_length };

	path.next();

	while (true)
	{
		input[path.m_pos] = 'X';
		path.next();

		try
		{
			if (input.at(path.m_pos) == '#')
			{
				path.turn();
			}
			else if (input.at(path.m_pos) == '\n')
			{
				break;
			}
		}
		catch (std::out_of_range)
		{
			break;
		}
	}

	std::vector<size_t> traversed;
	traversed.push_back(guard);

	for (const auto [index, c] : std::views::enumerate(input))
	{
		if (c == 'X')
		{
			traversed.push_back(index);
		}
	}

	const size_t max_steps = 10000;

	size_t obstacles = 0;
	std::mutex mutex;

	{
		Timer t;
		std::for_each(std::execution::par, traversed.begin(), traversed.end(), [&](const size_t index)
			{
				size_t steps = 0;
				std::string board{ input };

				board[index] = '#';

				Pathing path{ guard, line_length };

				while (true)
				{
					path.next();

					if (path.m_pos < 0 || path.m_pos >= board.size())
					{
						break;
					}

					if (board[path.m_pos] == '#')
					{
						path.turn();
					}
					else if (board[path.m_pos] == '\n')
					{
						break;
					}

					++steps;

					if (steps == max_steps)
					{
						std::lock_guard<std::mutex> guard(mutex);
						++obstacles;
						break;
					}
				}
			});
	}

	std::println("{}", obstacles);

}

