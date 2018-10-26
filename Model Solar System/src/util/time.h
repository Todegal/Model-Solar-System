// time.h -
// Handles time management using std::chrono::high_resolution_clock
#ifndef TIME_H
#define TIME_H

// STL includes
#include <ctime>
#include <ratio>
#include <chrono>

namespace timer
{
	static auto initTime = std::chrono::high_resolution_clock::now();

	// Resets the start point of the program to now
	static void resetTime()
	{
		initTime = std::chrono::high_resolution_clock::now();
	}

	// Returns the time since epoch in milliseconds
	template<typename T>
	static T timeSinceInit()
	{
		// Get time difference
		auto a = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() -
			initTime);

		// Cast to and return in milliseconds
		std::chrono::duration<T, std::milli> b = a;

		return b.count();
	}
}

#endif // !TIME_H
