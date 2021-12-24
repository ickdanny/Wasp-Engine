#pragma once

#include "WicBitmapStorage.h"
#include "D2DBitmapStorage.h"

namespace gameresource {
	struct ResourceMasterStorage {
		WicBitmapStorage wicBitmapStorage;
		D2DBitmapStorage d2dBitmapStorage;
	};
}