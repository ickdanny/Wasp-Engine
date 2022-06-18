#pragma once

#include <tuple>

#include "ECS/CriticalOrders.h"
#include "Game/Components.h"

namespace wasp::game::systems {

	//utility for creating entities

	//Base struct
	struct ComponentTupleBase {
		virtual ecs::entity::EntityHandle addTo(ecs::DataStorage& dataStorage) const;
	};

	//ComponentTuple struct
	template <typename... Ts>
	struct ComponentTuple : public std::tuple<Ts...> {
		ComponentTuple(const Ts&... args)
			: std::tuple<Ts...>{ args... } {
		}

		ComponentTuple(std::tuple<Ts...> tuple)
			: std::tuple<Ts...>(tuple) {
		}

		//conversion to AddEntityOrder
		ecs::AddEntityOrder<Ts...> package() const {
			return ecs::AddEntityOrder{ *this };
		}

		//override addTo
		ecs::entity::EntityHandle addTo(ecs::DataStorage& dataStorage) const override {
			return dataStorage.addEntity(package());
		}
	};

	//adding anything to the tuple
	template <typename... Ts, typename C>
	ComponentTuple<Ts..., C> operator+(
		const ComponentTuple<Ts...>& tuple, 
		const C& component
	) {
		return std::tuple_cat(
			static_cast<std::tuple<Ts...>>(tuple), 
			std::make_tuple(component)
		);
	}

	//concatenating two tuples
	template <typename... Ts, typename... Us>
	ComponentTuple<Ts..., Us...> operator+(
		const ComponentTuple<Ts...>& left,
		const ComponentTuple<Us...>& right
	) {
		return std::tuple_cat(
			static_cast<std::tuple<Ts...>>(left), 
			static_cast<std::tuple<Us...>>(right)
		);
	}

	struct EntityBuilder {
	private:
		//typedefs
		using Point2 = math::Point2;
		using AABB = math::AABB;

	public:
		template <typename... Ts>
		static auto makeEntity(const Ts&... args) {
			return ComponentTuple{ args... };
		}

		template <typename... Ts>
		static auto makePosition(const Point2& pos, const Ts&... args) {
			return ComponentTuple{ Position{ pos }, args... };
		}

		template <typename... Ts>
		static auto makeVisible(const Point2& pos, const Ts&... args) {
			return ComponentTuple{ Position{ pos }, VisibleMarker{}, args... };
		}

		template <typename... Ts>
		static auto makeStationaryCollidable(
			const Point2& pos,
			const AABB& hitbox,
			const Ts&... args
		) {
			return ComponentTuple{
				Position{ pos },
				VisibleMarker{},
				Hitbox{ hitbox },
				CollidableMarker{},
				args...
			};
		}

		template <typename... Ts>
		static auto makeStationaryUncollidable(
			const Point2& pos,
			const AABB& hitbox,
			const Ts&... args
		) {
			return ComponentTuple{
				Position{ pos },
				VisibleMarker{},
				Hitbox{ hitbox }
			};
		}
	};
}