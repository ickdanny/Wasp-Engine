//From Subterranean Animism on, the games depend on a newer DirectX 9.0c 
//build not supported on Windows 98.
#pragma once

typedef struct IUnknown IUnknown;

#include "targetver.h"
#define WIN32_LEAN_AND_MEAN   // Exclude rarely-used stuff from Windows headers
// Windows Header Files
#include <windows.h>
#include <wincodec.h>
#include <shobjidl.h>
#include <atlbase.h>
#include <mmeapi.h>
#pragma comment(lib, "winmm")
#include <dwrite.h>
#pragma comment(lib, "Dwrite")
#include <d2d1.h>
#pragma comment(lib, "d2d1")

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>