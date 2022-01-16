#pragma once

#include <string>
#include <memory>

#include "ResourceBase.h"
#include "ResourceOriginVariant.h"

namespace wasp::resource {
	template<typename T>
	class Resource : public ResourceBase {
	protected:
		ResourceOriginVariant origin{};
		std::shared_ptr<T> dataPointer{};

	public:
		Resource(
			const std::wstring& id,
			const ResourceOriginVariant& origin
		)
			: ResourceBase{ id, false }
			, origin{ origin } {
		}

		Resource(
			const std::wstring& id,
			const ResourceOriginVariant& origin,
			std::shared_ptr<T> dataPointer
		)
			: ResourceBase{ id, true }
			, origin{ origin }
			, dataPointer{ dataPointer } {
		}

		const ResourceOriginVariant& getOrigin() const {
			return origin;
		}
		std::shared_ptr<T> getDataPointerCopy() {
			return dataPointer;
		}
		const std::shared_ptr<const T> getDataPointerCopy() const {
			return dataPointer;
		}
		void setData(std::shared_ptr<T> dataPointer) {
			this->dataPointer = dataPointer;
			loaded = static_cast<bool>(dataPointer);
		}
		void unloadData() {
			dataPointer = nullptr;
			loaded = false;
		}
	};
}