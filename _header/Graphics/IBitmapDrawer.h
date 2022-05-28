#pragma once

#include "IDrawer.h"
#include "BitmapDrawInstruction.h"
#include "Math\Point2.h"
#include "Math\Rectangle.h"

namespace wasp::graphics {
	class IBitmapDrawer : public virtual IDrawer {
	public:
		virtual void drawBitmap(
			const math::Point2 center,
			const BitmapDrawInstruction& bitmapDrawInstruction
		) = 0;

		virtual void drawSubBitmap(
			const math::Point2 center,
			const BitmapDrawInstruction& bitmapDrawInstruction,
			const math::Rectangle& sourceRectangle
		) = 0;
	};
}