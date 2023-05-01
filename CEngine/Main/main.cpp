#include "CEngine/Game/Game.h"

#include <string>

// process input -> update game -> render

int WINAPI WinMain(_In_ HINSTANCE hInstance,
                   _In_opt_ HINSTANCE hPrevInstance,
                   _In_ PSTR lpCmdLine,
                   _In_ int nCmdShow) {
    Game game;

    return game.Start();
}