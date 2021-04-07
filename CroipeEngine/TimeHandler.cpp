#ifndef TIME_HANDLER
#define TIME_HANDLER
/* Contains Functions For Handling Time And Getting Elapsed Time */
#include <chrono>

namespace Croipe::TimeHandler {
	/* Globals */
	std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	
	/* Functions */
	void ClockStart() {
		/* Set Start */
		start = std::chrono::steady_clock::now();
	}

	void ClockEnd() {
		/* Set End */
		end = std::chrono::steady_clock::now();
	}

	float ElaspedTime() {
		/* Get Difference Between Start And End, And Return As A Decimal Seconds Value */
		return std::chrono::duration_cast<std::chrono::microseconds>(start - end).count() * 0.000001f;
	}
}

#endif