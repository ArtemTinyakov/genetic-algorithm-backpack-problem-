#ifndef SINGLETON
#define SINGLETON

#include <vector>
#include "log.h"

class singleton;

class singletonDestroyer {
	singleton* p_instance;

	public:

	~singletonDestroyer();
	
	void initialize(singleton* p);
};

class singleton {
	static singleton* p_instance;
	static singletonDestroyer destr;
	static std::vector<std::pair<double, double>> items; //list of weights and costs of items
	static double mut_prob; //probability of mutation
	static int max_wei; //maximum weight in backpack
	static int n_items; //number of items at all
	static int max_geners; //maximum number of generations
	static int r_seed; //random seed
	static int sz_popul; //size of population
	static Log logfile; //log

	protected:

	singleton();
	singleton(const singleton&) = delete;
	singleton& operator=(const singleton&) = delete;
	~singleton() = default;
	friend class singletonDestroyer;

	public:

	static singleton& get_instance();
	static const std::vector<std::pair<double, double>>& get_items();
	static const double& get_mut_prob();
	static const int& get_max_wei();
	static const int& get_n_items();
	static const int& get_max_geners();
	static const int& get_r_seed();
	static const int& get_sz_popul();
	static Log& get_logfile();
};

#endif SINGLETON