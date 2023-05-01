#pragma once

#include "CEngine/Window/Window.h"
#include "CEngine/Timer/Timer.h"

class Game {
public:
	Game();
	~Game() = default;
	// cannot be copied
	Game(const Game&) = delete;
	Game& operator= (const Game&) = delete;

private:	
	Window m_wnd;
	Timer m_timer;

public:
	int Start();

private:
	void DoFrameStart();
};