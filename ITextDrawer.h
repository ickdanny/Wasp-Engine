#pragma once

#include <string>
#include <vector>

#include "IDrawer.h"
#include "Point2.h"

namespace graphics {
	class ITextDrawer : public virtual IDrawer {
	public:
		virtual void drawText(
			geometry::Point2 pos,
			const std::wstring& text
		) = 0;
	};

	//todo: when loading dialogue, split lines into vectors of wstrings
	//todo: see ImageUtil.textToImage()
}