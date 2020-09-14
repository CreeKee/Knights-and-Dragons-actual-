#pragma once
#include "Window.h"
#include "Timer.h"
#include <sstream>
#include <iomanip>
#include <optional>
#include <random>

class App
{

private:
	Window wnd;
	Timer time;
	std::vector<std::unique_ptr<class Box>> boxes;

	void doFrame();

public:
	App();
	~App();
	int go();
};