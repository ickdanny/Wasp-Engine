#pragma once

#include <vector>

#include "ArchetypeIterator.h"

namespace wasp::ecs::component {

	template <typename... Ts>
	class GroupIterator {
	private:
		//typedefs
		using InnerIteratorType = ArchetypeIterator<Ts...>;
	public:
		using ReturnType = InnerIteratorType::ReturnType;

	private:
		//fields
		int currentIteratorPairIndex{};
		//held in pairs of current/end iterators
		std::vector<std::pair<InnerIteratorType>> innerIterators;

	public:
		GroupIterator(std::vector<std::pair<InnerIteratorType>> innerIterators)
			: currentIteratorPairIndex{ 0 }
			, innerIterators { innerIterators } {
		}

		int getEntityID() {
			return getCurrentIterator().getEntityID();
		}

		ReturnType operator*() const {
			return *getCurrentIterator()
		}

		bool isValid() {
			return currentIteratorPairIndex < innerIterators.size();
		}

		//prefix increment
		GroupIterator& operator++() {
			++getCurrentIterator();
			if (getCurrentIterator() == getCurrentEndIterator()) {
				++currentIteratorPairIndex;
			}
			return *this;
		}

		//postfix increment
		GroupIterator operator++(int) {
			GroupIterator temp{ *this };
			++(*this);
			return temp;
		}

		//no point in equality operators because we use isValid instead

	private:
		InnerIteratorType& getCurrentIterator() {
			return std::get<0>(innerIterators[currentIteratorPairIndex]);
		}

		InnerIteratorType& getCurrentEndIterator() {
			return std::get<1>(innerIterators[currentIteratorPairIndex]);
		}
	};
}