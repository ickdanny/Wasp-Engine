#pragma once

#include <tuple>

#include "Container/IntLookupTable.h"

namespace wasp::ecs::component {

	template <typename... Ts>
	class MultiComponentIterator {
	private:
		//typedefs
		template<typename T>
		using innerIteratorType = container::IntLookupTable<T>::Iterator;
	public:
		using iterator_category = std::forward_iterator_tag;
		using difference_type = typename std::vector<T>::iterator::difference_type;
		using value_type = std::tuple<Ts>;
		using pointer = value_type*;
		using reference = value_type&;

	private:
		//fields
		std::tuple<innerIteratorType<Ts>> innerIteratorTuple{};

	public:
		MultiComponentIterator(std::tuple<innerIteratorType<Ts>> innerIteratorTuple)
			: innerIteratorTuple{ innerIteratorTuple } {
		}

		int getPreviousEntityID() {
			return std::get<0>(innerIteratorTuple).getPreviousSparseIndex();
		}

		//operators
		reference operator*() const {
			//todo: how do i make a tuple
			return *valueIterator;
		}
		pointer operator->() {
			return valueIterator;
		}

		//prefix increment
		MultiComponentIterator& operator++() {
			//todo: how do i increment every inner iterator?
			++valueIterator;
			++currentDenseIndex;
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