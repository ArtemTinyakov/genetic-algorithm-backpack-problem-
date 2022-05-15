#include "individual.h"
#include "singleton.h"
#include <cstdlib>
#include <fstream>
#include <iostream>

individual::individual() : stuff{}
{
//	#pragma omp parallel for            not working
	for (int i = 0; i < n_items; ++i)
		stuff.push_back(rand() % 2);
}

individual::~individual() = default;

double individual::fitness()
{
	double w{.0}, c{.0};
//	#pragma omp parallel for
	for (int i = 0; i < stuff.size(); ++i)
		if (stuff[i])
		{
			w += singleton::get_items()[i].first;
			c += singleton::get_items()[i].second;
		}
	return (w > max_w) ? 0 : c;
}

std::vector<bool> individual::get_stuff()
{
	return stuff;
}

void individual::mutation()
{
//	#pragma omp parallel for
	for (int i = 0; i < stuff.size(); ++i)
	{
		if (rand() % 100 < (mut_prob * 100))
			stuff[i] = !stuff[i];
	}
}

void individual::print()
{
	for (auto e : stuff)
		std::cout << e << ' ';
	std::cout << '\n';
}

void swap(individual& individ1, individual& individ2, int i)
{
	bool rab = individ1.stuff[i];
	individ1.stuff[i] = individ2.stuff[i];
	individ2.stuff[i] = rab;
}

std::pair<individual, individual> crossing(individual individ1, individual individ2)
{
	std::pair<individual, individual> res;
	res.first = individ1;
	res.second = individ2;
	int cut_point = rand() % n_items;
	#pragma omp parallel for
	for (int i = cut_point; i < n_items; ++i)
	{
		swap(res.first, res.second, i);
	}
	return res;
}

individual individual::clone()
{
	individual new_ind;
	new_ind.stuff = stuff;
	return new_ind;
}