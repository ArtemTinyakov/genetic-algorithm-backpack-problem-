#ifndef INDIVIDUAL
#define INDIVIDUAL

#include <vector>
#include <string>

class individual{
	std::vector<bool> stuff;
	int flag;  //0 - starting individ, 1 - was a kid, 2 - was mutated, -1 - wasn't exist
	int prev_flag;

	public:

		individual();
		~individual() = default;

		int fitness();
		void mutation();
		individual clone();
		void print();
		std::vector<bool> get_stuff();
		std::string to_string();
		int get_flag();
		void set_flag(int);
		int get_prev_flag();
		void set_prev_flag(int);


		friend std::pair<individual, individual> crossing(individual, individual);
};

#endif //INDIVIDUAL