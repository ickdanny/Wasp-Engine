//From Subterranean Animism on, the games depend on a newer DirectX 9.0c 
//build not supported on Windows 98.
#pragma once

#define NOMINMAX

typedef struct IUnknown IUnknown;

#include "targetver.h"
#define WIN32_LEAN_AND_MEAN   // Exclude rarely-used stuff from Windows headers
// Windows Header Files
#include <windows.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>