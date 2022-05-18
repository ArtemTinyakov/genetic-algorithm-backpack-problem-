#include "graphics.h"
#include "singleton.h"
#include <iostream>
#include <algorithm>

graphics::graphics()
{
	init();
}

graphics::~graphics()
{
	quit();
}

void graphics::init()
{
	window = SDL_CreateWindow("genetic algorithm", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, singleton::get_width(), singleton::get_height(), SDL_WINDOW_SHOWN);
	if (!window)
		std::cout << "can't create window. SDL error : " << SDL_GetError() << '\n';
	ren = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (!ren)
		std::cout << "can't create renderer. SDL error : " << SDL_GetError() << '\n';
}

void graphics::quit()
{
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(ren);
	SDL_Quit();
}

void graphics::print_population(int npop)
{
	std::vector<std::pair<int, int>> pos = positions(npop);
	for (int i = 0; i < fit.size(); ++i)
	{
		SDL_Rect rect{pos[i].first, pos[i].second, individ_size().first, individ_size().second};
		set_color(make_color(fit[i]).first, make_color(fit[i]).second);
		SDL_RenderFillRect(ren, &rect);
		SDL_RenderPresent(ren);
	}
}

void graphics::set_population(std::vector<individual> popul)
{
	fit.clear();
	fit.resize(0);
	for (auto e : popul)
		fit.push_back(e.fitness());
}

void graphics::clear_screen()
{
	set_color(0x00, 0x00);
	SDL_Rect rect{ 0,0,singleton::get_width(), singleton::get_height() };
	SDL_RenderFillRect(ren, &rect);
	SDL_RenderPresent(ren);
}

void graphics::set_color(Uint8 r, Uint8 g)
{
	SDL_SetRenderDrawColor(ren, r, g, 0x00, 0x00);
}

std::vector<std::pair<int, int>> graphics::positions(int ngen)
{
	std::vector<std::pair<int, int>> res;
	std::pair<int, int> indiv_size{ individ_size() };
	for (int i = 0; i < singleton::get_width() / indiv_size.first; ++i)
		res.push_back({ (indiv_size.first * i) % singleton::get_width(), (indiv_size.second * ngen) % singleton::get_height() + ngen });
	return res;
}

std::pair<int, int> graphics::individ_size()
{
	return { singleton::get_width() / fit.size(), singleton::get_height() / (singleton::get_max_geners()+4) };
}

std::pair<Uint8, Uint8> graphics::make_color(int fitness)
{
	double d = (double(fitness) / /*fit[max_element(fit.begin(), fit.end()) - fit.begin()]*/29) * 1.6;
	//говно, но не знаю пока как сделать по-другому
	if (d <= 0.1)
		return { 0xFF, 0x00 };
	else if (d <= 0.2)
		return { 0xEE, 0x11 };
	else if (d <= 0.3)
		return { 0xDD, 0x22 };
	else if (d <= 0.4)
		return { 0xCC, 0x33 };
	else if (d <= 0.5)
		return { 0xBB, 0x44 };
	else if (d <= 0.6)
		return { 0xAA, 0x55 };
	else if (d <= 0.7)
		return { 0x99, 0x66 };
	else if (d <= 0.8)
		return { 0x88, 0x77 };
	else if (d <= 0.9)
		return { 0x77, 0x88 };
	else if (d <= 1.0)
		return { 0x66, 0x99 };
	else if (d <= 1.1)
		return { 0x55, 0xAA };
	else if (d <= 1.2)
		return { 0x44, 0xBB };
	else if (d <= 1.3)
		return { 0x33, 0xCC };
	else if (d <= 1.4)
		return { 0x22, 0xDD };
	else if (d <= 1.5)
		return { 0x11, 0xEE };
	return { 0x00, 0xFF };
}