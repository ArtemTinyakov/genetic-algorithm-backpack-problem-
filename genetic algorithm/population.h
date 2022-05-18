#ifndef POPULATION
#define POPULATION

#include "individual.h"
#include <vector>
#include <string>

class population {
	std::vector<individual> popul;

	public:

	population();
	~population() = default;
	void set_population(std::vector<individual>);
	std::vector<individual> get_population();
	void concat(population);
	void add(individual);
	void mutation();
	std::string fit_to_str();
	void print_fit();
	void print_pop_log();
	double max_fit();
	void best_individs_origin(int);
	double p_cross(int);
	bool will_cross(double, double);
	bool will_cross(int, int);
	bool will_cross_actual(int, int);
	void reduce_population();
	void tournament();
	individual best_individ();
	double mean_fit();

	protected:

	int max3_fit(int, int, int);
};

#endif POPULATION