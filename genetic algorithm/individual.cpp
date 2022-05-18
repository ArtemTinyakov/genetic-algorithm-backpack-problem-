#include "individual.h"
#include "singleton.h"
#include "log.h"
#include <cstdlib>
#include <fstream>
#include <iostream>

individual::individual() : stuff(singleton::get_n_items(), -1), flag{0}, prev_flag{-1}
{
//	std::cout << "creating individual...\n";
	#pragma omp parallel for
	for (int i = 0; i < singleton::get_n_items(); ++i)
		stuff[i] = bool(rand() % 2);
}

int individual::fitness()
{
//	singleton::get_logfile().printLog("getting fitness of individual...");
	int w{0}, c{0};
	#pragma omp parallel for 
	for (int i = 0; i < stuff.size(); ++i)
		if (stuff[i])
		{
			w += singleton::get_items()[i].first;
			c += singleton::get_items()[i].second;
		}
	return (w > singleton::get_max_wei()) ? 0 : c;
}

void individual::mutation()
{
	int fit = fitness();
	bool is_mutated{ false };
//	singleton::get_logfile().printLog("individual is mutating...");
//	#pragma omp parallel for
	for (int i = 0; i < stuff.size(); ++i)
		if (rand() % 100 < (singleton::get_mut_prob() * 100))
		{
			stuff[i] = !stuff[i];
			is_mutated = true;
		}
	if (is_mutated)
	{
		set_prev_flag(flag);
		set_flag(2);
	}
/*	if (fit < fitness())
		std::cout << "individ get better fitness function via mutation!\n";
*/
}

individual individual::clone()
{
//	singleton::get_logfile().printLog("cloning individual...");
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
//	singleton::get_logfile().printLog("getting stuff of individual...");
	return stuff;
}

std::string individual::to_string()
{
	std::string res;
	for (int i = 0; i < stuff.size(); ++i)
		res += std::to_string(stuff[i]);
	return res;
}

int individual::get_flag()
{
	return flag;
}

void individual::set_flag(int f)
{
	flag = f;
}

int individual::get_prev_flag()
{
	return prev_flag;
}

void individual::set_prev_flag(int pf)
{
	prev_flag = pf;
}

std::pair<individual, individual> crossing(individual individ1, individual individ2)
{
	singleton::get_logfile().printLog("crossing individuals " + individ1.to_string() + " and " + individ2.to_string());
	std::pair<individual, individual> res{individ1, individ2};
	int cut_point = rand() % singleton::get_n_items();
	#pragma omp parallel for
	for (int i = cut_point; i < singleton::get_n_items(); ++i)
	{
		bool rab = res.first.stuff[i];
		res.first.stuff[i] = res.second.stuff[i];
		res.second.stuff[i] = rab;
	}
	res.first.set_flag(1);
	res.second.set_flag(1);
	res.first.set_prev_flag(-1);
	res.second.set_prev_flag(-1);
	return res;
}