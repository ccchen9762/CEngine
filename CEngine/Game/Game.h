#pragma once

#include "CEngine/Window/Window.h"
#include "CEngine/Timer/Timer.h"
#include "CEngine/Imgui/ImguiManager.h"


class Game {
public:
	Game();
	~Game() = default;
	// cannot be copied
	Game(const Game&) = delete;
	Game& operator= (const Game&) = delete;

private:
	ImguiManager imgui; // order matters!! Imgui need to be initialized before Window
	Window m_wnd;
	Timer m_timer;

public:
	int Start();

private:
	void DoFrameStart();
};