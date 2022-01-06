#pragma once

#include "framework.h"
#include <stdexcept>

namespace wasp::graphics {
	class BitmapDrawInstruction {
		
	private:
		static constexpr float defaultRotationDegrees{ 0.0f };
		static constexpr float defaultOpacity{ 1.0f };
		static constexpr float defaultScale{ 1.0f };

		CComPtr<ID2D1Bitmap> bitmap{};
		float rotationDegrees{ defaultRotationDegrees };
		float opacity{ defaultOpacity };
		float scale{ defaultScale };

	public:
		BitmapDrawInstruction() = default;
		BitmapDrawInstruction(
			CComPtr<ID2D1Bitmap> bitmap,
			float rotationDegrees = defaultRotationDegrees,
			float opacity = defaultOpacity,
			float scale = defaultScale
		)
			: bitmap{ bitmap }
			, rotationDegrees{ rotationDegrees }
			, opacity{ opacity }
			, scale{ scale }
		{
			throwIfOutOfRange();
		}

		CComPtr<ID2D1Bitmap> getBitmap() {
			return bitmap;
		}

		const CComPtr<ID2D1Bitmap> getBitmap() const {
			return bitmap;
		}

		float getRotationDegrees() const {
			return rotationDegrees;
		}

		float getOpacity() const {
			return opacity;
		}

		float getScale() const {
			return scale;
		}

		void setBitmap(CComPtr<ID2D1Bitmap> bitmap) {
			this->bitmap = bitmap;
		}

		void setRotationDegrees(float rotationDegrees) {
			this->rotationDegrees = rotationDegrees;
		}

		void setOpacity(float opacity) {
			this->opacity = opacity;
			throwIfOpacityOutOfRange();
		}

		void setScale(float scale) {
			this->scale = scale;
			throwIfScaleOutOfRange();
		}

		bool requiresRotation() const {
			return rotationDegrees != defaultRotationDegrees;
		}

		bool requiresScale() const {
			return scale != defaultScale;
		}

	private:
		void throwIfOutOfRange() const {
			throwIfOpacityOutOfRange();
			throwIfScaleOutOfRange();
		}
		void throwIfOpacityOutOfRange() const {
			if (opacity < 0.0f || opacity > 1.0f) {
				throw std::out_of_range("Error opacity out of range");
			}
		}
		void throwIfScaleOutOfRange() const {
			if (scale <= 0.0f) {
				throw std::out_of_range("Error scale out of range");
			}
		}
	};
}