#ifndef LOGGER_H
#define LOGGER_H
#include "../globaldata.h"

class Logger
{
	private:

	public:
		static void log(std::string message, std::string sender = "deltaXi")
		{
			std::cout << sender + ": " + message << std::endl;
		}

		static void logExtra(std::string message, std::string sender = "deltaXi")
		{
			globals::state s = globals::currentState;
			std::cout << sender + ": " + "currentState: " + std::to_string(s) + " - msg: " + message << std::endl;
		}

		template <class anyType>
		static void logTypeLess(anyType message, std::string sender = "deltaXi")
		{
			globals::state s = globals::currentState;
			std::string msg = std::to_string(message);
			std::cout << sender + ": " + "currentState: " + std::to_string(s) + " - msg: " + msg << std::endl;
		}
};

#endif