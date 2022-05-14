#ifndef INDIVIDUAL
#define INDIVIDUAL

#include "constants.h"
#include <vector>
#include <string>

class individual{
	std::vector<bool> stuff;

	public:

		individual();
		~individual();

		double fitness();
		void mutation();
		individual clone();
		void print();

		friend std::pair<individual, individual> crossing(individual, individual);
		friend void swap(individual&, individual&, int);
};

#endif //INDIVIDUAL