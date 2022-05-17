#ifndef GRAPHICS
#define GRAPHICS

#include <vector>
#include <SDL.h>
#include "individual.h"

class graphics {
	std::vector<int> fit;
	SDL_Window* window = nullptr;
	SDL_Renderer* ren = nullptr;

	public:

	graphics();
	~graphics();

	void init();
	void quit();
	void print_population(int);
	void set_population(std::vector<individual>);
	void clear_screen();

	protected:

	void set_color(Uint8, Uint8);
	std::vector<std::pair<int, int>> positions(int);
	std::pair<int, int> individ_size();
	std::pair<Uint8, Uint8> make_color(int);
};

#endif GRAPHICS