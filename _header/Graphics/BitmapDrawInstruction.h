#pragma once

#include "windowsInclude.h"
#include "windowsD2DInclude.h"
#include <atlbase.h>		//for CComPtr
#include <stdexcept>

#include "Math/Angle.h"
#include "Math/Vector2.h"

namespace wasp::graphics {
	class BitmapDrawInstruction {
	private:
		//typedefs
		using Angle = math::Angle;
		using Vector2 = math::Vector2;

		//constants
		static constexpr float minOpacity{ 0.0f };
		static constexpr float maxOpacity{ 1.0f };
		static constexpr float minScale{ 0.01f };
		static constexpr float defaultRotation{ 0.0f };
		static constexpr float defaultOpacity{ 1.0f };
		static constexpr float defaultScale{ 1.0f };

		//fields
		CComPtr<ID2D1Bitmap> bitmap{};
		Vector2 offset{};
		Angle rotation{ defaultRotation };
		float opacity{ defaultOpacity };
		float scale{ defaultScale };

		bool updated{};         //useful for implementation

	public:
		//constructor
		BitmapDrawInstruction(
			const CComPtr<ID2D1Bitmap>& bitmap,
			const Vector2& offset = {},
			Angle rotation = { defaultRotation },
			float opacity = defaultOpacity,
			float scale = defaultScale
		)
			: bitmap{ bitmap }
			, offset{ offset }
			, rotation{ rotation }
			, opacity{ opacity }
			, scale{ scale }
		{
			throwIfOutOfRange();
		}

		//getters
		const CComPtr<ID2D1Bitmap>& getBitmap() const {
			return bitmap;
		}
		const Vector2& getOffset() const {
			return offset;
		}
		Angle getRotation() const {
			//return by value
			return rotation;
		}
		float getOpacity() const {
			return opacity;
		}
		float getScale() const {
			return scale;
		}

		//setters
		void setBitmap(const CComPtr<ID2D1Bitmap>& bitmap) {
			this->bitmap = bitmap;
			flagForUpdate();
		}
		void setOffset(const Vector2& offset) {
			this->offset = offset;
			flagForUpdate();
		}
		void setRotation(Angle rotation) {
			this->rotation = rotation;
			flagForUpdate();
		}
		void setOpacity(float opacity) {
			this->opacity = opacity;
			throwIfOpacityOutOfRange();
			flagForUpdate();
		}
		void setScale(float scale) {
			this->scale = scale;
			throwIfScaleOutOfRange();
			flagForUpdate();
		}

		bool requiresRotation() const {
			return static_cast<float>(rotation) != defaultRotation;
		}

		bool requiresScale() const {
			return scale != defaultScale;
		}

		//updating
		bool isUpdated() const {
			return updated;
		}
		void flagForUpdate() {
			updated = false;
		}
		void update() {
			updated = true;
		}

	private:
		void throwIfOutOfRange() const {
			throwIfOpacityOutOfRange();
			throwIfScaleOutOfRange();
		}
		void throwIfOpacityOutOfRange() const {
			if (opacity < minOpacity || opacity > maxOpacity) {
				throw std::out_of_range("Error opacity out of range");
			}
		}
		void throwIfScaleOutOfRange() const {
			if (scale <= minScale) {
				throw std::out_of_range("Error scale out of range");
			}
		}
	};
}