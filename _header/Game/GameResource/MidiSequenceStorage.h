#pragma once

#include "Resource\ResourceStorage.h"
#include "Resource\ResourceBase.h"
#include "Sound/MidiSequence.h"

#pragma warning(disable : 4250) //suppress inherit via dominance

namespace wasp::game::gameresource {
	class MidiSequenceStorage 
		: public resource::ResourceStorage<sound::midi::MidiSequence>
		, public resource::FileLoadable
		, public resource::ManifestLoadable 
	{
	private:
		using MidiSequence = sound::midi::MidiSequence;
		using ResourceType = resource::Resource<MidiSequence>;

	public:
		MidiSequenceStorage()
			: FileLoadable{ {L"mid"} }
			, ManifestLoadable{ {L"midi"} } {
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