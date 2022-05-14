#ifndef SINGLETON
#define SINGLETON

#include <vector>

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
	static std::vector<std::pair<double, double>> items;

	protected:

	singleton();
	singleton(const singleton&) = delete;
	singleton& operator=(const singleton&) = delete;
	~singleton() = default;
	friend class singletonDestroyer;

	public:

	static singleton& get_instance();
	static const std::vector<std::pair<double, double>>& get_items();
};

#endif SINGLETON