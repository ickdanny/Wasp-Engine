#include "Game/Systems/ScriptSystem.h"

#include "Game/Systems/ComponentOrderQueue.h"

namespace wasp::game::systems {

	using namespace ecs;
	using namespace ecs::entity;

	namespace {

		#define NODE_HANDLER_ARGS \
			Scene& scene, \
			EntityID entityID, \
			std::shared_ptr<components::ScriptNode>& currentScriptNodePointer, \
			std::unordered_map<components::ScriptNode*, void*>& externalData, \
			ComponentOrderQueue& componentOrderQueue

		bool runScriptNode(NODE_HANDLER_ARGS) {
			switch (currentScriptNodePointer->scriptInstruction) {
				case components::ScriptInstructions::error:
					throw std::runtime_error{ "error script instruction" };


				default:
					throw std::runtime_error{ "unhandled script instruction!" };
			}
		}

		void runScriptProgramList(
			Scene& scene,
			ScriptProgramList& scriptProgramList,
			EntityID entityID,
			ComponentOrderQueue& componentOrderQueue
		) {
			for (auto itr{ scriptProgramList.begin() }; itr != scriptProgramList.end();){
				auto& scriptProgram{ *itr };
				while (scriptProgram.currentNodePointer) {
					if (!runScriptNode(
						scene,
						entityID,
						scriptProgram.currentNodePointer,
						scriptProgram.externalData,
						componentOrderQueue
					)) {
						break;
					}
				}
				if(!scriptProgram.currentNodePointer) {
					itr = scriptProgramList.erase(itr);
				}
			}
		}

		#undef NODE_HANDLER_ARGS
	}

	void ScriptSystem::operator()(Scene& scene) {
		//create our component order queue
		ComponentOrderQueue componentOrderQueue{};

		//get the group iterator for ScriptProgramList
		static const Topic<ecs::component::Group*> groupPointerStorageTopic{};
		auto groupPointer{
			getGroupPointer<ScriptProgramList>(
				scene,
				groupPointerStorageTopic
			)
		};
		auto groupIterator{ groupPointer->groupIterator<ScriptProgramList>() };

		//populate our component order queue with every component order this tick
		while (groupIterator.isValid()) {
			auto [scriptProgramList] = *groupIterator;
			EntityID entityID{ groupIterator.getEntityID() };
			runScriptProgramList(
				scene, 
				scriptProgramList, 
				entityID, 
				componentOrderQueue
			);
			++groupIterator;
		}

		//apply all our queued component orders to the ecs world
		componentOrderQueue.apply(scene.getDataStorage());
	}
}