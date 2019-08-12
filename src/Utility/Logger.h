#ifndef LOGGER_H
#define LOGGER_H
#include "../globaldata.h"
#include "PCH.hpp"

class Logger
{
	private:

	public:
		static void log(std::string message, std::string sender = "deltaXi")
		{
			std::cout << sender + ": " + message << std::endl;
		}

		static void log(std::wstring message, std::string sender = "deltaXi")
		{
			std::cout << sender + ": ";
			std::wcout << message << std::endl;
		}

		static void log(int data, std::string sender = "deltaXi")
		{
			std::cout << sender + ": " + std::to_string(data) << std::endl;
		}

		static void log(float data, std::string sender = "deltaXi")
		{
			std::cout << sender + ": " + std::to_string(data) << std::endl;
		}

		static void log(sf::Vector2f data, std::string sender = "deltaXi")
		{
			std::cout << sender + ": " + "X=" + std::to_string(data.x) + " - Y=" + std::to_string(data.y) << std::endl;
		}

		static void log(sf::Vector2i data, std::string sender = "deltaXi")
		{
			std::cout << sender + ": " + "X=" + std::to_string(data.x) + " - Y=" + std::to_string(data.y) << std::endl;
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