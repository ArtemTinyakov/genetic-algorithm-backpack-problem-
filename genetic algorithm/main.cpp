#include <iostream>
#include <omp.h>
#include <algorithm>
#include <cmath>
#include "individual.h"
#include "singleton.h"

bool comp(individual i1, individual i2)
{
	return i1.fitness() > i2.fitness();
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
	double res{ 0 };
	#pragma omp parallel for
	for (int i = 0; i < popul.size(); ++i)
		res = (res > popul[i].fitness()) ? res : popul[i].fitness();
	return res;
}

double p_cross(individual indiv, std::vector<individual> popul)
{
	return indiv.fitness() / max_fit(popul);
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
	return (d1 != 0 && d2 != 0) ? 2 / (1 / d1 + 1 / d2) : 0;
}

double mean_arithm(double d1, double d2)
{
	return (d1 + d2) / 2;
}

double mean_geom(double d1, double d2)
{
	return sqrt(d1 * d2);
}

bool will_cross(double d1, double d2)
{
	return ((double(rand() % 100) / 100) < mean_geom(d1, d2)) ? true : false; //can use mean_arith, mean_geom or mean_harmonic
}

void reduce_population(std::vector<individual>& popul)
{
	popul.erase(popul.begin() + sz_popul, popul.end());
}

individual max3_fit(individual i1, individual i2, individual i3)
{
	double maxfit = max3(i1.fitness(), i2.fitness(), i3.fitness());
	if (maxfit == i1.fitness())
		return i1;
	else if (maxfit == i2.fitness())
		return i2;
	return i3;
}

std::vector<individual> tournament(std::vector<individual> population)
{
	std::vector<individual> new_p;
//	#pragma omp parallel for            not working
	for (int i = 0; i < sz_popul; ++i)
	{
		int i1{ 0 }, i2{ 0 }, i3{ 0 };
		while ((i1 == i2) || (i2 == i3) || (i1 == i3))
		{
			i1 = rand() % sz_popul;
			i2 = rand() % sz_popul;
			i3 = rand() % sz_popul;
		}
		new_p.push_back(max3_fit(population[i1], population[i2], population[i3]));
	}
	return new_p;
}

individual best_individ(std::vector<individual> popul)
{
	int ind{ 0 }, fit{ 0 };
	for (int i = 0; i < popul.size(); ++i)
		ind = (popul[i].fitness() > fit) ? i : ind;
	return popul[ind];
}

int main()
{
	singleton::get_instance();
	std::vector<std::pair<double, double>> items = singleton::get_items();
	srand(r_seed);
	std::vector<individual> population;
	for (int i = 0; i < sz_popul; ++i)
		population.push_back(individual());
/*	std::vector<double> fit(sz_popul);
	#pragma omp parallel for
	for (int i = 0; i < sz_popul; ++i)
		fit[i] = population[i].fitness();
*/
/*	for (auto e : population)
	{
		e.print();
	}
*/
/*	for (auto e : fit)
		std::cout << e << '\n';
*/
	for (int i = 0; i < max_geners; ++i)
	{
		std::vector<individual> kids;
		population = tournament(population);
		for (int j = 0; j < sz_popul-1; ++j)
		{
			for (int k = j+1; k < sz_popul; ++k)
			{
				if (will_cross(p_cross(population[j], population), p_cross(population[k], population)))
				{
					std::pair<individual, individual> kid = crossing(population[j], population[k]);
					kids.push_back(kid.first);
					kids.push_back(kid.second);
				}
			}
		}
		population.insert(population.end(), kids.begin(), kids.end());
		std::sort(population.begin(), population.end(), comp);
		reduce_population(population);
		for (int i = 0; i < sz_popul; ++i)
			population[i].mutation();
		std::cout << "generation " << i << ", best fitness == " << best_individ(population).fitness() << '\n';
	}
	std::cout << "backpack filled with " << best_individ(population).fitness();
	return 0;
}