#pragma once

#include <chrono>

class Timer {
public:
	Timer();
	~Timer() = default;

	float Elapse();
	float Peek();

private:
	std::chrono::steady_clock::time_point m_start;
	std::chrono::steady_clock::time_point m_last;
};