#ifndef LOG
#define LOG

#include <fstream>
#include <string>

class Log {
	std::ofstream out;

	public:

	Log();
	~Log() = default;

	void printLog(std::string);
	void closeLog();
};

#endif LOG