#pragma once

#include "IDrawer.h"
#include "BitmapDrawInstruction.h"
#include "Point2.h"

namespace graphics {
	class IBitmapDrawer : public virtual IDrawer {
	public:
		virtual void drawBitmap(
			const geometry::Point2& center,
			const BitmapDrawInstruction& bitmapDrawInstruction
		) = 0;
	};
}