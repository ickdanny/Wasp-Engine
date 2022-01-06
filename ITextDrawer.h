#pragma once

#include <string>
#include <vector>

#include "IDrawer.h"
#include "Point2.h"

namespace wasp::graphics {
	class ITextDrawer : public virtual IDrawer {
	public:
		virtual void drawText(
			const geometry::Point2 pos,
			const std::wstring& text,
			const std::pair<float, float> bounds
		) = 0;
	};
}