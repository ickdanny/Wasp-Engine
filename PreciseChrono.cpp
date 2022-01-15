#include "PreciseChrono.h"

#include "framework.h"

namespace wasp::utility {
	bool sleep100ns(long long hundredNanoseconds) {
		//implementation from https://gist.github.com/Youka/4153f12cf2e17a77314c

		if (hundredNanoseconds <= 0) {
			return true;
		}

		HANDLE timerHandle{ CreateWaitableTimer(NULL, TRUE, NULL) };
		if (!timerHandle) {
			return false;
		}

		LARGE_INTEGER time{};
		time.QuadPart = -hundredNanoseconds;

		if (!SetWaitableTimer(timerHandle, &time, 0, NULL, NULL, FALSE)) {
			CloseHandle(timerHandle);
			return false;
		}

		//start and wait for timer
		WaitForSingleObject(timerHandle, INFINITE);
		//clean up timer
		CloseHandle(timerHandle);

		return true;
	}
}