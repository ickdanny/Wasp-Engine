#pragma once

#include <memory>
#include "framework.h"

#include "ResourceStorage.h"
#include "IResource.h"

namespace gameresource {

	struct WicBitmapAndChildD2DBitmap {
		CComPtr<IWICFormatConverter> wicBitmap;
		std::weak_ptr<resource::IResource> d2dChildResource;
	};

	class WicBitmapStorage
		: public resource::ResourceStorage<WicBitmapAndChildD2DBitmap>
		, public resource::FileLoadable 
	{

	};
}