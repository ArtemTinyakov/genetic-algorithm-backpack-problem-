#include "population.h"
#include "singleton.h"
#include <iostream>

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

population::population()
{
	for (int i = 0; i < singleton::get_sz_popul() + singleton::get_max_children(); ++i)
		popul.push_back(individual());
}

void population::set_population(std::vector<individual> p)
{
	popul = p;
}

std::vector<individual> population::get_population()
{
	return popul;
}

void population::concat(population p)
{
	std::vector<individual> rab = p.get_population();
	popul.insert(popul.end(), rab.begin(), rab.end());
}

void population::add(individual i)
{
	popul.push_back(i);
}

void population::mutation()
{
	for (int i = singleton::get_sz_popul(); i < popul.size(); ++i)
		popul[i].mutation();
}

std::string population::fit_to_str()
{
	std::string res;
	for (int i = 0; i < popul.size(); ++i)
		res += std::to_string(popul[i].fitness()) + " ";
	return res;
}

void population::print_fit()
{
	for (auto e : popul)
		std::cout << e.fitness() << ' ';
	std::cout << '\n';
}

double population::max_fit()
{
//	singleton::get_logfile().printLog("finding individual with maximal fitness in population...");
	double res{ 0 };
	#pragma omp parallel for
	for (int i = 0; i < popul.size(); ++i)
		res = (res > popul[i].fitness()) ? res : popul[i].fitness();
//	singleton::get_logfile().printLog("done");
	return res;
}

void population::best_individs_origin(int ngen)
{
	std::cout << "generation" << '[' << ngen << "]\n";
	for (auto e : popul)
	{
		if (e.fitness() == max_fit())
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

double population::p_cross(int i)
{
	singleton::get_logfile().printLog("calculating probability of crossing of individ " + popul[i].to_string() + "...\n");
	double res = popul[i].fitness() / max_fit();
	singleton::get_logfile().printLog("success, p_cross = " + std::to_string((res > 0) ? res : 0.2));
	return (res > 0) ? res : 0.2;
}

bool population::will_cross(double d1, double d2)
{
	singleton::get_logfile().printLog("calculating will individuals with p_crosses " + std::to_string(d1) + " and " + std::to_string(d2) + " cross...\ndone");
	return ((double(rand() % 100) / 100) < mean_geom(d1, d2)) ? true : false; //can use mean_arith, mean_geom or mean_harmonic
}

bool population::will_cross(int i1, int i2)
{
	//	singleton::get_logfile().printLog("calculating will individuals " + i1.to_string() + " and " + i2.to_string() + " cross...");
	double difers{ .0 };
	std::vector<bool> stuff1 = popul[i1].get_stuff(), stuff2 = popul[i2].get_stuff();
	for (int i = 0; i < singleton::get_n_items(); ++i)
		if (stuff1[i] != stuff2[i])
			++difers;
	//	singleton::get_logfile().printLog("done");
	return ((double(rand() % 100) / 100) < (((difers / singleton::get_n_items()) < 0.2) ? 0.2 : double(difers / singleton::get_n_items()))) ? true : false;
}

bool population::will_cross_actual(int i1, int i2)
{
	return (popul[i1].fitness() >= mean_fit() && popul[i2].fitness() >= mean_fit()) ? true : false;
}

void population::reduce_population()
{
	singleton::get_logfile().printLog("reducing population...");
	popul.erase(popul.begin() + singleton::get_sz_popul(), popul.end());
	singleton::get_logfile().printLog("done");
}

double max3(double d1, double d2, double d3)
{
	if (d1 >= d2 && d1 >= d3)
		return d1;
	else if (d2 >= d1 && d2 >= d3)
		return d2;
	return d3;
}

int population::max3_fit(int i1, int i2, int i3)
{
	singleton::get_logfile().printLog("finding individual with maximum fitness of " + popul[i1].to_string() + " " + popul[i2].to_string() + " " + popul[i3].to_string() + "...");
	double maxfit = max3(popul[i1].fitness(), popul[i2].fitness(), popul[i3].fitness());
	if (maxfit == popul[i1].fitness())
		return i1;
	else if (maxfit == popul[i2].fitness())
		return i2;
	return i3;
}

void population::tournament()
{
	singleton::get_logfile().printLog("starting tournament...");
	population new_p;
	new_p.set_population({});
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
		new_p.add(popul[max3_fit(i1, i2, i3)]);
	}
	singleton::get_logfile().printLog("tournament finished");
	set_population(new_p.get_population());
}

individual population::best_individ()
{
	//	singleton::get_logfile().printLog("finding best individual of population...");
	int ind{ 0 };
	int fit{ 0 };
	for (int i = 0; i < popul.size(); ++i)
		if (popul[i].fitness() > fit)
		{
			ind = i;
			fit = popul[i].fitness();
		}
	//	singleton::get_logfile().printLog("done");
	return popul[ind];
}

double population::mean_fit()
{
	double res{ .0 };
	for (auto e : popul)
		res += e.fitness();
	res /= popul.size();
	return res;
}