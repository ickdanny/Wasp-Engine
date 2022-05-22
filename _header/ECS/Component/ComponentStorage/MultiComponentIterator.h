#pragma once

#include <tuple>

#include "Container/IntLookupTable.h"

namespace wasp::ecs::component {

	//does not actually qualify as an iterator
	template <typename... Ts>
	class MultiComponentIterator {
	private:
		//typedefs
		template<typename T>
		using innerIteratorType = typename container::IntLookupTable<T>::Iterator;
	public:
		using returnType = std::tuple<Ts&&...>;

	private:
		//fields
		std::tuple<innerIteratorType<Ts>...> innerIteratorTuple{};

	public:
		MultiComponentIterator(std::tuple<innerIteratorType<Ts>...> innerIteratorTuple)
			: innerIteratorTuple{ innerIteratorTuple } {
		}

		int getPreviousEntityID() {
			return std::get<0>(innerIteratorTuple).getPreviousSparseIndex();
		}

		returnType operator*() const {
			return std::apply(
				[&](auto& ...x) {
					return std::forward_as_tuple((*x, ...));
				},
				innerIteratorTuple
			);
		}

		//prefix increment
		MultiComponentIterator& operator++() {
			std::apply(
				[&](auto& ...x) {
					(++x, ...);
				}, 
				innerIteratorTuple
			);
			return *this;
		}

		//postfix increment
		MultiComponentIterator operator++(int) {
			MultiComponentIterator temp{ *this };
			++(*this);
			return temp;
		}

		friend bool operator== (
			const MultiComponentIterator& a, 
			const MultiComponentIterator& b
		) {
			return a.innerIteratorTuple == b.innerIteratorTuple;
		};
		friend bool operator!= (
			const MultiComponentIterator& a, 
			const MultiComponentIterator& b
		) {
			return a.innerIteratorTuple != b.innerIteratorTuple;
		};
	};
}