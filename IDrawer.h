#pragma once

namespace graphics {
	class IDrawer {
	public:
		virtual void beginDraw() = 0;
		virtual void endDraw() = 0;
	};
}