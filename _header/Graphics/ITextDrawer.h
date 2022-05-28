#pragma once

#include <string>
#include <vector>

#include "IDrawer.h"
#include "Math\Point2.h"

namespace wasp::graphics {
	class ITextDrawer : public virtual IDrawer {
	public:
		virtual void drawText(
			const math::Point2 pos,
			const std::wstring& text,
			const std::pair<float, float> bounds
		) = 0;
	};
}