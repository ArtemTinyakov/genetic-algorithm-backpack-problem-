#ifndef INDIVIDUAL
#define INDIVIDUAL

#include <vector>
#include <string>

class individual{
	std::vector<bool> stuff;

	public:

		individual();
		~individual();

		int fitness();
		void mutation();
		individual clone();
		void print();
		std::vector<bool> get_stuff();
		std::string to_string();


		friend std::pair<individual, individual> crossing(individual, individual);
};

#endif //INDIVIDUAL