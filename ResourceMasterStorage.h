#pragma once

#include "DirectoryStorage.h"
#include "BitmapStorage.h"

namespace gameresource {
	struct ResourceMasterStorage {
		DirectoryStorage directoryStorage;
		BitmapStorage bitmapStorage;
	};
}