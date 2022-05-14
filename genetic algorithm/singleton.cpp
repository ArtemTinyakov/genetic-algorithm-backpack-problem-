#include "singleton.h"
#include "constants.h"
#include <fstream>
#include <iostream>

singleton* singleton::p_instance = 0;
singletonDestroyer singleton::destr;
std::vector<std::pair<double, double>> singleton::items;

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
//	std::cout << "getting items...\n";
	return items;
}