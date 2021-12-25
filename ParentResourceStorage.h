#pragma once

#include "Resource.h"
#include "ResourceStorage.h"

namespace gameresource {
	using ChildList = std::vector<resource::IResource*>;

	class ChildListResource
		: public resource::Resource<std::vector<resource::IResource*>> {

	public:
		ChildListResource(
			const std::wstring& id,
			const resource::ResourceOriginVariant& origin
		)
			: resource::Resource<ChildList>{ id, origin } {
		}

		ChildListResource(
			const std::wstring& id,
			const resource::ResourceOriginVariant& origin,
			std::shared_ptr<ChildList> dataPointer
		)
			: resource::Resource<ChildList>{
				id, origin, dataPointer
		} {
		}

	protected:
		void removeChild(resource::IResource* child) override;
	};

	class ParentResourceStorage : public resource::ResourceStorage<ChildList>{

	public:

		void unload(const std::wstring& id) override;
		void remove(const std::wstring& id) override;
	};
}