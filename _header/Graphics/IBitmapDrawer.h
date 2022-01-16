#pragma once

#include "IDrawer.h"
#include "BitmapDrawInstruction.h"
#include "Geometry\Point2.h"
#include "Geometry\Rectangle.h"

namespace wasp::graphics {
	class IBitmapDrawer : public virtual IDrawer {
	public:
		virtual void drawBitmap(
			const geometry::Point2 center,
			const BitmapDrawInstruction& bitmapDrawInstruction
		) = 0;

		virtual void drawSubBitmap(
			const geometry::Point2 center,
			const BitmapDrawInstruction& bitmapDrawInstruction,
			const geometry::Rectangle& sourceRectangle
		) = 0;
	};
}