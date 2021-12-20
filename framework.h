#pragma once

typedef struct IUnknown IUnknown;

#include "targetver.h"
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files
#include <windows.h>
#include <wincodec.h>
#include <shobjidl.h>
#include <atlbase.h>
#include <d2d1.h>
#pragma comment(lib, "d2d1")

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>