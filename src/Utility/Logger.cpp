#include "Logger.h"
#include "PCH.hpp"

void Logger::log(std::string message, std::string sender)
{
	std::cout << sender + ": " + message << std::endl;
}

void Logger::logExtra(std::string message, std::string sender)
{
	globals::state s = globals::currentState;
	std::cout << sender + ": " + "currentState: " + std::to_string(s) + " - msg: " + message << std::endl;
}