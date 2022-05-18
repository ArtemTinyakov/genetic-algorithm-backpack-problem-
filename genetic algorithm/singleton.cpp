#include "singleton.h"
#include "log.h"
#include <fstream>
#include <iostream>

singleton* singleton::p_instance = 0;
singletonDestroyer singleton::destr;
std::vector<std::pair<int, int>> singleton::items;
double singleton::mut_prob = 0.2; //probability of mutation
int singleton::max_wei = 25; //maximum weight in backpack
int singleton::n_items = 10; //number of items at all
int singleton::max_geners = 50; //maximum number of generations
int singleton::r_seed = 1488; //random seed
int singleton::sz_popul = 15; //size of population
Log singleton::logfile;
int singleton::width = 640;
int singleton::height = 640;
int singleton::max_children = 15;

singleton::singleton()
{
	p_instance = 0;
	items.clear();
	items.resize(0);
	logfile.printLog("trying to read input file...");
	std::ifstream in("input.txt");
	if (in.is_open())
	{
		logfile.printLog("reading...");
		std::pair<int, int> rab;
		for (int i = 0; i < n_items; ++i)
		{
			in >> rab.first >> rab.second;
			items.push_back(rab);
			logfile.printLog("weight : " + std::to_string(rab.first) + ", cost : " + std::to_string(rab.second));
		}
	}
	else
		logfile.printLog("input file is unavailable, closing...");
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

const std::vector<std::pair<int, int>>& singleton::get_items()
{
//	logfile.printLog("getting items weight-cost list...");
	return items;
}

const double& singleton::get_mut_prob()
{
//	logfile.printLog("getting mutation probability...");
	return mut_prob;
}

const int& singleton::get_max_wei()
{
//	logfile.printLog("getting maximum weight the backpack can carry...");
	return max_wei;
}

const int& singleton::get_n_items()
{
//	logfile.printLog("getting number of items...");
	return n_items;
}

const int& singleton::get_max_geners()
{
//	logfile.printLog("getting number of generations...");
	return max_geners;
}

const int& singleton::get_r_seed()
{
//	logfile.printLog("getting seed for randomizer...");
	return r_seed;
}

const int& singleton::get_sz_popul()
{
//	logfile.printLog("getting size of population...");
	return sz_popul;
}

Log& singleton::get_logfile()
{
	return logfile;
}

const int& singleton::get_width()
{
	return width;
}

const int& singleton::get_height()
{
	return height;
}

const int& singleton::get_max_children()
{
	return max_children;
}