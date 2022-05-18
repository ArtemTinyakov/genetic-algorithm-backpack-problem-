#include <iostream>
#include <omp.h>
#include <algorithm>
#include <cmath>
#include "individual.h"
#include "singleton.h"
#include "graphics.h"
#include <SDL.h>
#include <ctime>

#undef main

//--------------------graphics--------------------//

void init_sdl()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
		std::cout << "SDL initialization failed. SDL error " << SDL_GetError() << '\n';
	else
		std::cout << "SDL initialization succeed!\n";
}

void test1()
{
	init_sdl();
	const int max_width{ 1536 }, max_height{ 824 };
	SDL_Window* window = nullptr;
	SDL_Surface* scrSurface = nullptr;
	window = SDL_CreateWindow("SDL test", 0, 0, max_width, max_height, SDL_WINDOW_SHOWN);
	if (!window)
		std::cout << "Window wasn't created. SDL error " << SDL_GetError() << '\n';
	else
	{
		scrSurface = SDL_GetWindowSurface(window);
		SDL_Rect rect{ 100, 100, 100, 300 };
		SDL_FillRect(scrSurface, nullptr, SDL_MapRGB(scrSurface->format, 0x0, 0x0, 0x0));
		SDL_FillRect(scrSurface, &rect, SDL_MapRGB(scrSurface->format, 0xFF, 0x0, 0x0));
		SDL_UpdateWindowSurface(window);
		SDL_Delay(1000);
	}
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void test2()
{
	init_sdl();
	const int max_width{ 1536 }, max_height{ 824 };
	SDL_Window* window = nullptr;
	SDL_Surface* scrSurface1 = nullptr;
	SDL_Surface* scrSurface2 = nullptr;
	window = SDL_CreateWindow("SDL test", SDL_WINDOWPOS_CENTERED_MASK, SDL_WINDOWPOS_CENTERED_MASK, 500, 500, SDL_WINDOW_SHOWN);
	if (!window)
		std::cout << "Window wasn't created. SDL error " << SDL_GetError() << '\n';
	else
	{
		scrSurface1 = SDL_GetWindowSurface(window);
	}
	scrSurface2 = SDL_LoadBMP("C:/pics/kekw.bmp");
	if (!scrSurface2)
		std::cout << "can't load picture. SDL error " << SDL_GetError() << '\n';
	else
	{
		SDL_FillRect(scrSurface1, nullptr, SDL_MapRGB(scrSurface1->format, 0xFF, 0xFF, 0xFF));
		SDL_Rect rect{ 100, 100, 200, 200 };
		SDL_BlitSurface(scrSurface2, nullptr, scrSurface1, &rect);
		SDL_UpdateWindowSurface(window);
		SDL_Delay(3000);
	}
	SDL_FreeSurface(scrSurface1);
	SDL_FreeSurface(scrSurface2);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

//------------------------------------------------//

bool comp(individual i1, individual i2)
{
	return i1.fitness() > i2.fitness();
}

std::string pop_fit_to_str(std::vector<individual> popul)
{
	std::string res;
	for (int i = 0; i < popul.size(); ++i)
		res += std::to_string(popul[i].fitness()) + " ";
	return res;
}

void print_fit_pop(std::vector<individual> popul)
{
	for (auto e : popul)
		std::cout << e.fitness() << ' ';
	std::cout << '\n';
}

double max3(double d1, double d2, double d3)
{
	if (d1 >= d2 && d1 >= d3)
		return d1;
	else if (d2 >= d1 && d2 >= d3)
		return d2;
	return d3;
}

double max_fit(std::vector<individual> popul)
{
//	singleton::get_logfile().printLog("finding individual with maximal fitness in population...");
	double res{ 0 };
	#pragma omp parallel for
	for (int i = 0; i < popul.size(); ++i)
		res = (res > popul[i].fitness()) ? res : popul[i].fitness();
//	singleton::get_logfile().printLog("done");
	return res;
}

void check_best_individs(std::vector<individual> popul, int ngen)
{
	std::cout << "generation" << '[' << ngen << "]\n";
	for (auto e : popul)
	{
		if (e.fitness() == max_fit(popul))
		{
			if (e.get_flag() == 0)
				std::cout << "starting\n";
			if (e.get_flag() == 1)
			{
				if (e.get_prev_flag() == 1)
					std::cout << "was a kid\n";
				else
					std::cout << "kid\n";
			}
			if (e.get_flag() == 2)
				std::cout << "mutated\n";
		}
	}
}

double p_cross(individual indiv, std::vector<individual> popul)
{
	singleton::get_logfile().printLog("calculating probability of crossing of individ " + indiv.to_string() + "...\n");
	double res = indiv.fitness() / max_fit(popul);
	singleton::get_logfile().printLog("success, p_cross = " + std::to_string((res > 0) ? res : 0.2));
	return (res > 0) ? res : 0.2;
/*
	double min_fit = indiv.fitness();
	#pragma omp parallel for
	for (auto e : popul)
		min_fit = (min_fit < e.fitness()) ? min_fit : e.fitness();
	double fit_sum{ 0 };
	#pragma omp parallel for
	for (auto e : popul)
		fit_sum += e.fitness();
	return ((indiv.fitness() - min_fit) / (fit_sum * min_fit));
*/
}

double mean_harmonic(double d1, double d2)
{
	singleton::get_logfile().printLog("calculating mean harmonic of " + std::to_string(d1) + " and " + std::to_string(d2) + "...\ndone");
	return (d1 != 0 && d2 != 0) ? 2 / (1 / d1 + 1 / d2) : 0;
}

double mean_arithm(double d1, double d2)
{
	singleton::get_logfile().printLog("calculating mean arithmetical of " + std::to_string(d1) + " and " + std::to_string(d2) + "...\ndone");
	return (d1 + d2) / 2;
}

double mean_geom(double d1, double d2)
{
	singleton::get_logfile().printLog("calculating mean geometrical of " + std::to_string(d1) + " and " + std::to_string(d2) + "...\ndone");
	return sqrt(d1 * d2);
}

bool will_cross(double d1, double d2)
{
	singleton::get_logfile().printLog("calculating will individuals with p_crosses " + std::to_string(d1) + " and " + std::to_string(d2) + " cross...\ndone");
	return ((double(rand() % 100) / 100) < mean_geom(d1, d2)) ? true : false; //can use mean_arith, mean_geom or mean_harmonic
}

bool will_cross(individual i1, individual i2)
{
//	singleton::get_logfile().printLog("calculating will individuals " + i1.to_string() + " and " + i2.to_string() + " cross...");
	double difers{ .0 };
	std::vector<bool> stuff1 = i1.get_stuff(), stuff2 = i2.get_stuff();
	for (int i = 0; i < singleton::get_n_items(); ++i)
		if (stuff1[i] != stuff2[i])
			++difers;
//	singleton::get_logfile().printLog("done");
	return ((double(rand() % 100) / 100) < (((difers / singleton::get_n_items()) < 0.2) ? 0.2 : double(difers/singleton::get_n_items()))) ? true : false;
}

bool will_cross(int fit1, int fit2, std::vector<individual> popul)
{
	return (fit1 >= max_fit(popul) && fit2 >= max_fit(popul)) ? true : false;
}

void reduce_population(std::vector<individual>& popul)
{
	singleton::get_logfile().printLog("reducing population...");
	popul.erase(popul.begin() + singleton::get_sz_popul(), popul.end());
	singleton::get_logfile().printLog("done");
}

individual max3_fit(individual i1, individual i2, individual i3)
{
	singleton::get_logfile().printLog("finding individual with maximum fitness of " + i1.to_string() + " " + i2.to_string() + " " + i3.to_string() + "...");
	double maxfit = max3(i1.fitness(), i2.fitness(), i3.fitness());
	if (maxfit == i1.fitness())
		return i1;
	else if (maxfit == i2.fitness())
		return i2;
	return i3;
}

std::vector<individual> tournament(std::vector<individual> popul)
{
	singleton::get_logfile().printLog("starting tournament...");
	std::vector<individual> new_p;
//	#pragma omp parallel for            //not working
	for (int i = 0; i < singleton::get_sz_popul(); ++i)
	{
		int i1{ 0 }, i2{ 0 }, i3{ 0 };
		while ((i1 == i2) || (i2 == i3) || (i1 == i3))
		{
			i1 = rand() % popul.size();
			i2 = rand() % popul.size();
			i3 = rand() % popul.size();
		}
		new_p.push_back(max3_fit(popul[i1], popul[i2], popul[i3]));
	}
	singleton::get_logfile().printLog("tournament finished");
	return new_p;
}

individual best_individ(std::vector<individual> popul)
{
//	singleton::get_logfile().printLog("finding best individual of population...");
	int ind{ 0 };
	double fit{ .0 };
	for (int i = 0; i < popul.size(); ++i)
		if (popul[i].fitness() > fit)
		{
			ind = i;
			fit = popul[i].fitness();
		}
//	singleton::get_logfile().printLog("done");
	return popul[ind];
}

int main(int argc, char* argv[])
{
	singleton::get_instance();
	singleton::get_logfile().printLog("Start of genetic algorithm");
	graphics graph;
/*	init_sdl();
	SDL_Window* window = nullptr;
	SDL_Renderer* ren = nullptr;
	window = SDL_CreateWindow("kekw", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_SHOWN);
	if (!window)
		std::cout << "can't create window. SDL error : " << SDL_GetError() << '\n';
	ren = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (!ren)
		std::cout << "can't create renderer. SDL error : " << SDL_GetError() << '\n';
	SDL_SetRenderDrawColor(ren, 0x00, 0xB0, 0x20, 0x00);
	SDL_Rect rect{ 100,100,50,90 };
	SDL_RenderFillRect(ren, &rect);
	SDL_RenderPresent(ren);
	SDL_Delay(2000);
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(ren);
	SDL_Quit();
*/
//	int start = clock();
//	std::cout << omp_get_max_threads() << '\n';
//	test2();
	std::vector<std::pair<int, int>> items = singleton::get_items();
//	int ndefect = (sz_popul < 10) ? 1 : sz_popul / 10;
	int max_children = singleton::get_sz_popul();
	singleton::get_logfile().printLog("maximum number of children = " + std::to_string(max_children));
	singleton::get_logfile().printLog("initializing randomizer...");
	srand(singleton::get_r_seed());
	std::vector<individual> population;
	singleton::get_logfile().printLog("creating start population...");
	for (int i = 0; i < singleton::get_sz_popul() + max_children; ++i)
		population.push_back(individual());
	singleton::get_logfile().printLog("done");
	graph.set_population(population);
	graph.print_population(0);
	SDL_Delay(500);
/*	std::vector<double> fit(sz_popul);
	#pragma omp parallel for
	for (int i = 0; i < sz_popul; ++i)
		fit[i] = population[i].fitness();
*/
/*	for (auto e : population)
	{
		e.print();
	}
	std::cout << '\n';
*/
	singleton::get_logfile().printLog("start population:");
	for (auto e : population)
	{
		singleton::get_logfile().printLog(e.to_string());
	}
/*	for (auto e : fit)
		std::cout << e << '\n';
*/
	for (int i = 0; i < singleton::get_max_geners(); ++i)
	{
		check_best_individs(population, i + 1);
		singleton::get_logfile().printLog("\ngeneration [" + std::to_string(i) + "]\n");
		std::vector<individual> kids;
		population = tournament(population);
		for (int j = 0; j < singleton::get_sz_popul() - 1; ++j)
		{
			for (int k = j+1; k < singleton::get_sz_popul(); ++k)
			{
//				if (will_cross(p_cross(population[j], population), p_cross(population[k], population)) && kids.size() < max_children)
//				if (will_cross(population[j], population[k]) && kids.size() < max_children)
				if (will_cross(population[j].fitness(), population[k].fitness(), population) && kids.size() < max_children)
				{
					std::pair<individual, individual> kid = crossing(population[j], population[k]);
					kids.push_back(kid.first);
					kids.push_back(kid.second);
					singleton::get_logfile().printLog("kids: " + kid.first.to_string() + " and " + kid.second.to_string());
				}
			}
		}
		population.insert(population.end(), kids.begin(), kids.end());
		singleton::get_logfile().printLog("population is mutating...");
		for (int i = singleton::get_sz_popul(); i < population.size(); ++i)
			population[i].mutation();
		singleton::get_logfile().printLog("done");
/*		std::sort(population.begin(), population.end(), comp);
		for (int i = 1; i <= ndefect; ++i)
			population[sz_popul - i] = population[population.size() - i];
		reduce_population(population);
*/
/*		for (auto e : population)
			e.print();

		std::cout << "fitnesses ";
		print_fit_pop(population);

		std::cout << "generation " << i << ", best fitness == " << best_individ(population).fitness() << '\n';
*/
		singleton::get_logfile().printLog("population:");
		for (auto e : population)
			singleton::get_logfile().printLog(e.to_string());
		singleton::get_logfile().printLog("population fitnesses:\n" + pop_fit_to_str(population));
		singleton::get_logfile().printLog("best fitness at the end of generation is " + std::to_string(best_individ(population).fitness()));
		graph.set_population(population);
		graph.print_population(i+1);
		SDL_Delay(500);
	}
//	std::cout << "backpack filled with " << best_individ(population).fitness() << '\n';
//	int end = clock();
//	std::cout << end - start;
	singleton::get_logfile().printLog("backpack filled with " + std::to_string(best_individ(population).fitness()));
	singleton::get_logfile().printLog("end of genetic algorithm");
	singleton::get_logfile().closeLog();
	SDL_Delay(5000);
	graph.quit();
	return 0;
}