#include "Utility\PreciseChrono.h"

#include "windowsInclude.h"
#include <stdexcept>

#include "Container/ObjectPool.h"

namespace wasp::utility {

	class TimerHandle100ns {
	private:
		HANDLE timerHandle{};
	public:
		TimerHandle100ns() 
			: timerHandle{ CreateWaitableTimer(NULL, TRUE, NULL) } {
			if (!timerHandle) {
				throw std::runtime_error{ "Error failed to open timer handle" };
			}
		}

		bool wait100ns(long long time100ns) {
			LARGE_INTEGER time{};
			time.QuadPart = -time100ns;

			if (!SetWaitableTimer(timerHandle, &time, 0, NULL, NULL, FALSE)) {
				return false;
			}

			//start and wait for timer
			WaitForSingleObject(timerHandle, INFINITE);

			return true;
		}

		~TimerHandle100ns() {
			CloseHandle(timerHandle);
		}
	};

	bool sleep100ns(long long time100ns) {
		static auto timerHandlePoolPointer{
			container::ObjectPool<TimerHandle100ns>::makeObjectPool() 
		};

		if (time100ns <= 0) {
			return true;
		}

		auto timerHandlePointer{ timerHandlePoolPointer->acquire() };
		return timerHandlePointer->wait100ns(time100ns);
	}
}