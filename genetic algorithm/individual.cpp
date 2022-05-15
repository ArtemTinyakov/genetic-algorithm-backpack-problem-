#include "individual.h"
#include "singleton.h"
#include <cstdlib>
#include <fstream>
#include <iostream>

individual::individual() : stuff{}
{
//	#pragma omp parallel for            not working
	for (int i = 0; i < singleton::get_n_items(); ++i)
		stuff.push_back(rand() % 2);
}

individual::~individual() = default;

double individual::fitness()
{
	double w{.0}, c{.0};
//	#pragma omp parallel for
	for (std::size_t i = 0; i < stuff.size(); ++i)
		if (stuff[i])
		{
			w += singleton::get_items()[i].first;
			c += singleton::get_items()[i].second;
		}
	return (w > singleton::get_max_wei()) ? 0 : c;
}

void individual::mutation()
{
//	#pragma omp parallel for
	for (std::size_t i = 0; i < stuff.size(); ++i)
	{
		if (rand() % 100 < (singleton::get_mut_prob() * 100))
			stuff[i] = !stuff[i];
	}
}

individual individual::clone()
{
	individual new_ind;
	new_ind.stuff = stuff;
	return new_ind;
}

void individual::print()
{
	for (auto e : stuff)
		std::cout << e << ' ';
	std::cout << '\n';
}

std::vector<bool> individual::get_stuff()
{
	return stuff;
}

std::pair<individual, individual> crossing(individual individ1, individual individ2)
{
	std::pair<individual, individual> res{individ1, individ1};
	int cut_point = rand() % singleton::get_n_items();
	#pragma omp parallel for
	for (int i = cut_point; i < singleton::get_n_items(); ++i)
		swap(res.first, res.second, i);
	return res;
}

void swap(individual& individ1, individual& individ2, int i)
{
	bool rab = individ1.stuff[i];
	individ1.stuff[i] = individ2.stuff[i];
	individ2.stuff[i] = rab;
}