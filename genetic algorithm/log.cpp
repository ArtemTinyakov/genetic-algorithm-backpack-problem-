#include "log.h"
#include <iostream>

Log::Log()
{
	out.open("log.txt");
}

void Log::printLog(std::string s)
{
	out << s << '\n';
}

void Log::closeLog()
{
	out.close();
}