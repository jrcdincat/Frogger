#include "pch.h"
#include <fstream>
#include <iostream>
#include "Game.h"

// Jonathan Conrad
// 12/7/2022
// Frogger Dependencies 
// vcpkg install sdl2 --triplet x64-windows
// vcpkg install sdl2-image --triplet x64 - windows
// vcpkg install sdl2-ttf --triplet x64 - windows
// vcpkg install sdl2-mixer --triplet x64 - windows

using namespace Frogger;

int main()
{
#if defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	Game game; 
	game.Run();
	while (game.IsRunning())
	{
		game.ProcessInput();
		game.Update();
		game.Render();
	}

	return EXIT_SUCCESS;
}