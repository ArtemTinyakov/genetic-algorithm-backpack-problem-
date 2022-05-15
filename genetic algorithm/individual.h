#ifndef INDIVIDUAL
#define INDIVIDUAL

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
		std::vector<bool> get_stuff();


		friend std::pair<individual, individual> crossing(individual, individual);
		friend void swap(individual&, individual&, int);
};

#endif //INDIVIDUAL