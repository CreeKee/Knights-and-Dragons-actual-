#pragma once
#include <chrono>

class Timer
{
public:
	Timer() noexcept;
	float mark() noexcept;
	float peek() const noexcept;
private:
	std::chrono::steady_clock::time_point last;
};