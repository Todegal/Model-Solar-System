// log.h -
// Handles printing to the console and error handling
#ifndef LOG_H
#define LOG_H

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <iostream>
#include <exception>
#include <string>
#include <sstream>
#include <typeinfo>

#include "time.h"

namespace logger
{
	// Static handle to the console
	static const HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	// Exception that stores the details and position of errors for storing and display
	class LoggerException : public std::exception
	{
	private:
		std::string message_, file_, function_;
		int line_;

	public:
		// Takes the file, function, and line of the exception and the message
		LoggerException(std::string message, std::string file, std::string function, int line) throw()
			: message_(message),
			file_(file),
			function_(function),
			line_(line)
		{ }

		// Return the appropriate values
		std::string getMessage() { return message_; }
		std::string getFile() { return file_; }
		std::string getFunction() { return function_; }
		int getLine() { return line_; }
	};

	// Print to the console in white with time stamp
	template<typename M, typename L, typename F>
	static void lgMessage(M message, L line, F function)
	{
#ifdef _DEBUG
		// Change the colour to white and print the message
		SetConsoleTextAttribute(logger::hConsole, 0x0F);
		std::cout << "MESSAGE(after " << timer::timeSinceInit<float>() / 1000 << "s): " << message << " (FUNCTION: " << function << ", LINE: " << line << ")" << std::endl;
#endif // _DEBUG
	}

	// Print warning to the console yellow with time stamp
	template<typename M, typename L, typename F>
	static void lgWarning(M warning, L line, F function)
	{
#ifdef _DEBUG
		// Change the colour to yellow and print the message
		SetConsoleTextAttribute(logger::hConsole, 0x0E);
		std::cout << "WARNING(after " << timer::timeSinceInit<float>() / 1000 << "s): " << warning << " (FUNCTION: " << function << ", LINE: " << line << ")" << std::endl;
#endif // _DEBUG
	}

	// Print error to the console red and with time stamp
	// Will also store the error as an exception
	template<typename M, typename L, typename F>
	static void lgError(M error, L line, F function, F file)
	{
#ifdef _DEBUG
		// Change the colour to red and print the error
		SetConsoleTextAttribute(logger::hConsole, 0x0C);
		std::cout << "ERROR(after " << timer::timeSinceInit<float>() / 1000 << "s): " << error << " (FUNCTION: " << function << ", LINE: " << line << ")" << std::endl;
#endif // _DEBUG

		// Convert the variables to the required types
		std::stringstream ss;

		ss << "ERROR(after " << timer::timeSinceInit<float>() / 1000 << "s): " << error;
		std::string e = ss.str();

		ss.str("");
		ss << function;
		std::string func = ss.str();

		ss.str("");
		ss << file;
		std::string f = ss.str();

		throw LoggerException(e, f, func, (int)line);
	}
}

// Prints message in white to the console
#define MESSAGE(message) logger::lgMessage(message, __LINE__, __FUNCTION__);

// Prints message in yellow to the console
#define WARNING(warning) logger::lgWarning(warning, __LINE__, __FUNCTION__);

// Prints message in red to the console and also terminates the program
#define ERROR(error) logger::lgError(error, __LINE__, __FUNCTION__, __FILE__);

#endif // !LOG_H
