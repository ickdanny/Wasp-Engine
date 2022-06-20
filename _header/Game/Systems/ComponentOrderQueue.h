#pragma once

#include "ECS/DataStorage.h"

namespace wasp::game::systems {

	//queues up add, set, and remove component orders
	class ComponentOrderQueue {
	private:
		//typedefs
		using EntityHandle = ecs::entity::EntityHandle;

		//inner types
		struct QueuedOrderBase {
			virtual ~QueuedOrderBase() = 0;
			virtual void apply(ecs::DataStorage& dataStorage) = 0;
		};

		template <typename T>
		struct QueuedAddComponentOrder : QueuedOrderBase {
			ecs::AddComponentOrder<T> addComponentOrder;

			void apply(ecs::DataStorage& dataStorage) override {
				dataStorage.addComponent(addComponentOrder);
			}
		};

		template <typename T>
		struct QueuedSetComponentOrder : QueuedOrderBase {
			ecs::SetComponentOrder<T> setComponentOrder;

			void apply(ecs::DataStorage& dataStorage) override {
				dataStorage.setComponent(setComponentOrder);
			}
		};

		template <typename T>
		struct QueuedRemoveComponentOrder : QueuedOrderBase {
			ecs::RemoveComponentOrder<T> removeComponentOrder;

			void apply(ecs::DataStorage& dataStorage) override {
				dataStorage.removeComponent(removeComponentOrder);
			}
		};

		//fields
		std::vector<std::unique_ptr<QueuedOrderBase>> queuedOrders{};

	public:

		template <typename T>
		void queueAddComponent(EntityHandle entityHandle, const T& component) {
			queuedOrders.emplace_back(
				std::move(
					std::unique_ptr<QueuedOrderBase>(
						new QueuedAddComponentOrder{ {entityHandle, component} }
					)
				)
			);
		}

		template <typename T>
		void queueSetComponent(EntityHandle entityHandle, const T& component) {
			queuedOrders.emplace_back(
				std::move(
					std::unique_ptr<QueuedOrderBase>(
						new QueuedAddComponentOrder{ {entityHandle, component} }
					)
				)
			);
		}

		template <typename T>
		void queueRemoveComponent(EntityHandle entityHandle) {
			queuedOrders.emplace_back(
				std::move(
					std::unique_ptr<QueuedOrderBase>(
						new QueuedRemoveComponentOrder<T>(entityHandle)
					)
				)
			);
		}

		void apply(ecs::DataStorage& dataStorage) {
			for (auto& order : queuedOrders) {
				order->apply(dataStorage);
			}
			queuedOrders.clear();
		}
	};
}