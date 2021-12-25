#include "ParentResourceStorage.h"

namespace gameresource {

	using ResourceType = resource::Resource<ChildList>;

	void ChildListResource::removeChild(resource::IResource* child) {
		auto found{ std::find(dataPointer->begin(), dataPointer->end(), child) };
		if (found != dataPointer->end()) {
			dataPointer->erase(found);
		}
		else {
			throw std::runtime_error("Error removeChild called on non child");
		}
	}

	static void unloadChildren(ResourceType& resource) {
		ChildList& childList{ *resource.getDataPointerCopy() };

		for (resource::IResource* childPointer : childList) {
			if (childPointer->isLoaded()) {
				const std::wstring& childID{ childPointer->getID() };
				childPointer->getStoragePointer()->unload(childID);
			}
		}
	}

	void ParentResourceStorage::unload(const std::wstring& id) {
		auto found{ resourceMap.find(id) };
		if (found != resourceMap.end()) {
			resource::Resource<ChildList>& resource{
				*(std::get<1>(*found))
			};

			if (resource.isLoaded()) {
				unloadChildren(resource);
				resource.unloadData();
			}
		}
	}

	static void removeChildren(ResourceType& resource) {
		ChildList& childList{ *resource.getDataPointerCopy() };

		for (resource::IResource* childPointer : childList) {
			if (childPointer->isLoaded()) {
				const std::wstring& childID{ childPointer->getID() };
				childPointer->getStoragePointer()->remove(childID);
			}
		}
	}

	void ParentResourceStorage::remove(const std::wstring& id) {
		auto found{ resourceMap.find(id) };
		if (found != resourceMap.end()) {
			resource::Resource<ChildList>& resource{
				*(std::get<1>(*found))
			};

			if (resource.isLoaded()) {
				removeChildren(resource);
				resourceMap.erase(found);
			}
		}
	}
}