#include "App.h"
#include "Box.h"
#include "memory"
#include <exception>

App::App()
	:
	wnd(1280, 720, "Knights and Dragons")
{
	std::mt19937 rng(std::random_device{}());
	std::uniform_real_distribution<float> adist(0.0f, 3.1415f * 2.0f);
	std::uniform_real_distribution<float> ddist(0.0f, 3.1415f * 2.0f);
	std::uniform_real_distribution<float> odist(0.0f, 3.1415f * 0.3f);
	std::uniform_real_distribution<float> rdist(6.0f, 20.0f);

	for (int i = 0; i < 100; i++) {
		boxes.push_back(std::make_unique<Box>(wnd.getGfx(), rng, adist, ddist, odist, rdist));
	}
	wnd.getGfx().setProjection(DirectX::XMMatrixPerspectiveLH(16.0f / 9.0f, 16.0f/9.0f, 1.0f, 3000.0f));
}

App::~App() {};

int App::go() {

	while (true){
		if (const auto ecode = Window::processMessages()) {
			return *ecode;
		}
		doFrame();
	}
}

void App::doFrame() {
	/*
	const float c = sin(time.peek()) / 2.0f + 0.5f;
	const float d = sin(time.peek()+((4.0 * acos(0.0))/3)) / 2.0f + 0.5f;
	const float e = sin(time.peek()+((8.0 * acos(0.0)) / 3)) / 2.0f + 0.5f;
	*/

	float dt = time.mark();

	wnd.getGfx().clearBuffer(0.07f, 0.0f, 0.12f);
	for (auto& b : boxes) {
		b->update(dt / 5.0f);
		b->draw(wnd.getGfx());
	}
	wnd.getGfx().endFrame();
}