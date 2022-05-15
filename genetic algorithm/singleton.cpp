#include "singleton.h"
#include <fstream>
#include <iostream>

singleton* singleton::p_instance = 0;
singletonDestroyer singleton::destr;
std::vector<std::pair<double, double>> singleton::items;
double singleton::mut_prob = 0.2; //probability of mutation
int singleton::max_wei = 25; //maximum weight in backpack
int singleton::n_items = 10; //number of items at all
int singleton::max_geners = 100; //maximum number of generations
int singleton::r_seed = 228; //random seed
int singleton::sz_popul = 5; //size of population

singleton::singleton()
{
	p_instance = 0;
	items.clear();
	items.resize(0);
	std::cout << "trying to read input file...\n";
	std::ifstream in("input.txt");
	if (in.is_open())
	{
		std::cout << "reading...\n";
		std::pair<double, double> rab;
		for (int i = 0; i < n_items; ++i)
		{
			in >> rab.first >> rab.second;
			items.push_back(rab);
			std::cout << "weight : " << rab.first << ", cost : " << rab.second << '\n';
		}
	}
	else
		std::cout << "input file is unavailable, closing...";
	in.close();
}

singletonDestroyer::~singletonDestroyer()
{
	delete p_instance;
}

void singletonDestroyer::initialize(singleton* p)
{
	p_instance = p;
}

singleton& singleton::get_instance()
{
	if (!p_instance)
	{
		p_instance = new singleton();
		destr.initialize(p_instance);
	}
	return *p_instance;
}

const std::vector<std::pair<double, double>>& singleton::get_items()
{
//	std::cout << "getting items weight-cost list...\n";
	return items;
}

const double& singleton::get_mut_prob()
{
//	std::cout << "getting mutation probability...\n";
	return mut_prob;
}

const int& singleton::get_max_wei()
{
//	std::cout << "getting maximum weight the backpack can carry...\n";
	return max_wei;
}

const int& singleton::get_n_items()
{
//	std::cout << "getting number of items...\n";
	return n_items;
}

const int& singleton::get_max_geners()
{
//	std::cout << "getting number of generations...\n";
	return max_geners;
}

const int& singleton::get_r_seed()
{
//	std::cout << "getting seed for randomizer...\n";
	return r_seed;
}

const int& singleton::get_sz_popul()
{
//	std::cout << "getting size of population";
	return sz_popul;
}