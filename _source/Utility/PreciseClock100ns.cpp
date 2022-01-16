#include "Utility/PreciseClock100ns.h"

#include "windowsInclude.h"

#include "HResultError.h"

namespace wasp::utility {

	namespace {
		long long queryPerformanceFrequency() {
			LARGE_INTEGER frequency{};
			if (!QueryPerformanceFrequency(&frequency)) {
				HRESULT result{ HRESULT_FROM_WIN32(GetLastError()) };
				throw win32adaptor::HResultError{
					"Error query performance frequency ", result
				};
			}
			return frequency.QuadPart;
		}

		long long queryPerformanceCounter() {
			LARGE_INTEGER performanceCount{};
			if (!QueryPerformanceCounter(&performanceCount)) {
				HRESULT result{ HRESULT_FROM_WIN32(GetLastError()) };
				throw win32adaptor::HResultError{
					"Error query performance counter ", result
				};
			}
			return performanceCount.QuadPart;
		}
	}

	PreciseClock100ns::PreciseClock100ns() {
		performanceFrequency = queryPerformanceFrequency();
	}
}