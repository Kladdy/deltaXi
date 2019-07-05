#ifndef LOGGER_H
#define LOGGER_H

class Logger
{
	private:

	public:
		static void log(std::string message, std::string sender = "deltaXi");
		static void logExtra(std::string message, std::string sender = "deltaXi");
};

#endif