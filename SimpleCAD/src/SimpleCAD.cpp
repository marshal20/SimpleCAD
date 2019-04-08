#include <iostream>
#include <SDL/SDL.h>

int main(int argc, char* argv[]) 
{
	SDL_Window* window;

	if (SDL_Init(SDL_INIT_VIDEO) < 0) 
	{
		std::cout << "Unable to initialise SDL\n" << SDL_GetError();
		return -1;
	}

	window = SDL_CreateWindow(
		"My SDL Game",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		640, 400, SDL_WINDOW_SHOWN);

	if (window == NULL) 
	{
		std::cout << "Unable to create a window\n" << SDL_GetError();
		return -1;
	}

	while (true)
	{
		SDL_PollEvent(NULL);
	}

	return 0;
}
