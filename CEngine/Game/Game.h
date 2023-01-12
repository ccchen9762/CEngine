#pragma once

#include "CEngine/Window/Window.h"
#include "CEngine/Timer/Timer.h"

class Game {
public:
	Game();
	~Game() = default;
	// not copyable
	Game(const Game&) = delete;
	Game& operator= (const Game&) = delete;

	int Start();

private:
	void DoFrameStart();

	Window m_wnd;
	//Timer m_timer;
};