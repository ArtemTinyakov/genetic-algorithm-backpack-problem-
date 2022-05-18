#include <iostream>
#include <omp.h>
#include <algorithm>
#include <cmath>
#include "individual.h"
#include "singleton.h"
#include "graphics.h"
#include "population.h"
#include <SDL.h>
#include <ctime>

#undef main

int main(int argc, char** argv)
{
	singleton::get_instance();
	singleton::get_logfile().printLog("Start of genetic algorithm");
	graphics graph;
	singleton::get_logfile().printLog("maximum number of children = " + std::to_string(singleton::get_max_children()));
	singleton::get_logfile().printLog("initializing randomizer...");
	srand(singleton::get_r_seed());
	population popul;
	singleton::get_logfile().printLog("creating start population...");
	singleton::get_logfile().printLog("done");
	graph.set_population(popul.get_population());
	graph.print_population(0);
	SDL_Delay(200);
	singleton::get_logfile().printLog("start population:");
	for (auto e : popul.get_population())
	{
		singleton::get_logfile().printLog(e.to_string());
	}
	for (int i = 0; i < singleton::get_max_geners(); ++i)
	{
		popul.best_individs_origin(i);
		singleton::get_logfile().printLog("\ngeneration [" + std::to_string(i) + "]\n");
		population kids;
		kids.set_population({});
		popul.tournament();
		for (int j = 0; j < singleton::get_sz_popul() - 1; ++j)
		{
			for (int k = j + 1; k < singleton::get_sz_popul(); ++k)
			{
				//if (popul.will_cross(p_cross(j), p_cross(k)) && kids.size() < max_children)
				//if (popul.will_cross(j, k) && kids.size() < max_children)
				if (popul.will_cross_actual(j, k) && kids.get_population().size() < singleton::get_max_children())
				{
					std::pair<individual, individual> kid = crossing(popul.get_population()[j], popul.get_population()[k]);
					kids.add(kid.first);
					kids.add(kid.second);
					singleton::get_logfile().printLog("kids: " + kid.first.to_string() + " and " + kid.second.to_string());
				}
			}
		}
		popul.concat(kids);
		singleton::get_logfile().printLog("population is mutating...");
		popul.mutation();
		singleton::get_logfile().printLog("done");
		singleton::get_logfile().printLog("population:");
		for (auto e : popul.get_population())
			singleton::get_logfile().printLog(e.to_string());
		singleton::get_logfile().printLog("population fitnesses:\n" + popul.fit_to_str());
		singleton::get_logfile().printLog("best fitness at the end of generation is " + std::to_string(popul.best_individ().fitness()));
		graph.set_population(popul.get_population());
		graph.print_population(i + 1);
		SDL_Delay(200);
	}
	singleton::get_logfile().printLog("backpack filled with " + std::to_string(popul.best_individ().fitness()));
	singleton::get_logfile().printLog("end of genetic algorithm");
	singleton::get_logfile().closeLog();
	SDL_Delay(5000);
	graph.quit();
	return 0;
}