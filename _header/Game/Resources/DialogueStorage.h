#pragma once

#include "Resource\ResourceStorage.h"
#include "Resource\ResourceBase.h"
#include "Dialogue.h"

#pragma warning(disable : 4250) //suppress inherit via dominance

namespace wasp::game::resources {
	class DialogueStorage
		: public resource::ResourceStorage<Dialogue>
		, public resource::FileLoadable
		, public resource::ManifestLoadable
	{
	private:
		using ResourceType = resource::Resource<Dialogue>;

	public:
		DialogueStorage()
			: FileLoadable{ {L"dlg"} }
			, ManifestLoadable{ {L"dialogue"} } {
		}

		void reload(const std::wstring& id) override;

		resource::ResourceBase* loadFromFile(
			const resource::FileOrigin& fileOrigin,
			const resource::ResourceLoader& resourceLoader
		) override;

		resource::ResourceBase* loadFromManifest(
			const resource::ManifestOrigin& manifestOrigin,
			const resource::ResourceLoader& resourceLoader
		) override;
	};
}