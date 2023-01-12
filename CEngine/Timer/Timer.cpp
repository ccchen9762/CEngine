#include "Timer.h"

Timer::Timer() {
	m_last = std::chrono::steady_clock::now();
}

float Timer::Elapse() {
	const std::chrono::steady_clock::time_point prev = m_last;
	m_last = std::chrono::steady_clock::now();
	const std::chrono::duration<float> timeElapsed = m_last - prev;

	return timeElapsed.count();
}

float Timer::Peek() {
	return std::chrono::duration<float>(std::chrono::steady_clock::now() - m_last).count();
}