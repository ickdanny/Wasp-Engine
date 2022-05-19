#pragma once

#include <vector>
#include <string>

namespace wasp::container {

	//An IntLookupTable is an int-indexed sparse set in which the values can only be
	//accessed via iteration

	//Nomenclature: 
	//A sparse index is an index of sparseIndices which maps to a dense index. 
	//A dense index is an index of denseValues which maps to a value.

	namespace {
		constexpr int invalidIndex{ -2 };
	}

	class IntLookupTableBase {};

	template <typename T>
	class IntLookupTable : public IntLookupTableBase{
	private:
		std::vector<int> sparseIndices{};
		std::vector<T> denseValues{};

		//denseIndexToSparseIndex maps a dense index back to it's corresponding sparse index
		std::vector<int> denseIndexToSparseIndex{};

		int currentSize{};

	public:

		IntLookupTable(const int initialMaxIndex, const int initialCapacity)
			: sparseIndices(initialMaxIndex)
			, denseValues(initialCapacity)
			, denseIndexToSparseIndex(initialCapacity)
			, currentSize{ 0 } 
		{
			clearSparseIndices();
			clearDenseIndexToSparseIndex();
		}

		int size() const {
			return currentSize;
		}

		bool contains(int sparseIndex) const {
			if (isInvalidSparseIndex(sparseIndex)) {
				return false;
			}
			return isValidDenseIndex(sparseIndices[sparseIndex])
		}

		//returns true if an element was replaced, false otherwise
		bool set(int sparseIndex, T value) {
			int denseIndex = sparseIndices[sparseIndex];
			if (isValidDenseIndex(denseIndex)) {
				denseValues[denseIndex] = value;
				return true;
			}
			else {
				appendToBack(sparseIndex, value);
				return false;
			}
		}

		//returns true if an element was removed, false otherwise
		bool remove(int sparseIndex) {
			//case 1: there is no such element -> return false
			if (isInvalidSparseIndex) {
				return false;
			}
			int denseIndex = sparseIndices[i];
			if (isInvalidDenseIndex(denseIndex)) {
				return false;
			}

			//case 2: there is an element -> return true
			//decrement currentSize first for the following functions to operate properly
			--currentSize;
			//if the element to remove is not the last, swap with the last element
			if (currentSize > 0 && denseIndex != currentSize) {
				overwriteWithElementAtCurrentSize(denseIndex);
			}
			//otherwise, we just removed the last element
			else {
				removeElementAtCurrentSize(currentSize);
			}
			return true;
		}

		void clear() {
			clearSparseIndices();
			clearDenseValues();
			clearDenseIndexToSparseIndex();
			currentSize = 0;
		}

		Iterator begin() {
			return Iterator{ 0 };
		}

		Iterator end() {
			return Iterator{ currentSize };
		}

		class Iterator {
		public:
			using iterator_category = std::forward_iterator_tag;
			using difference_type = std::vector::iterator::difference_type;
			using value_type = T;
			using pointer = T*;
			using reference = T&;

		private:
			//fields
			std::vector::iterator valueIterator{};
			int currentDenseIndex{};

		public:
			Iterator(int denseIndex)
				: valueIterator{ denseValues.begin() + denseIndex }
				, currentDenseIndex{ denseIndex } {
			}

			int getPreviousSparseIndex() {
				int previousDenseIndex = currentDenseIndex - 1;
				throwIfInvalidDenseIndex(previousDenseIndex);

				int previousSparseIndex = denseIndexToSparseIndex[previousDenseIndex];
				throwIfInvalidSparseIndex(previousSparseIndex);

				return previousSparseIndex;
			}

			//operators
			reference operator*() const { 
				return valueIterator*; 
			}
			pointer operator->() { 
				return valueIterator->; 
			}

			//prefix increment
			Iterator& operator++() { 
				++valueIterator; 
				++currentDenseIndex;
				return *this; 
			}

			//postfix increment
			Iterator operator++(int) { 
				Iterator temp = *this; 
				++(*this); 
				return temp; 
			}

			friend bool operator== (const Iterator& a, const Iterator& b) { 
				return (a.valueIterator == b.valueIterator)
					&& (a.currentDenseIndex == b.currentDenseIndex);
			};
			friend bool operator!= (const Iterator& a, const Iterator& b) { 
				return (a.valueIterator != b.valueIterator)
					|| (a.currentDenseIndex != b.currentDenseIndex);
			};
		};

	private:

		//clearing functions
		void clearSparseIndices() {
			std::fill(sparseIndices.begin(), sparseIndices.end(), invalidIndex);
		}
		void clearDenseValues() {
			denseValues.clear();
		}
		void clearDenseIndexToSparseIndex() {
			std::fill(
				denseIndexToSparseIndex.begin(), 
				denseIndexToSparseIndex.end(), 
				invalidIndex
			);
		}

		//index checker functions
		bool isValidSparseIndex(int sparseIndex) {
			return sparseIndex >= 0 && sparseIndex < sparseIndices.size();
		}
		bool isInvalidSparseIndex(int sparseIndex) {
			return !isValidSparseIndex(sparseIndex);
		}
		bool isValidDenseIndex(int denseIndex) {
			return denseIndex >= 0 && denseIndex < denseValues.size();
		}
		bool isInvalidDenseIndex(int denseIndex) {
			return !isValidDenseIndex(denseIndex);
		}

		//modification functions
		void removeElementAtCurrentSize() {
			throwIfInvalidDenseIndex(currentSize);

			invalidateSparseIndex(denseIndexToSparseIndex[currentSize]);
			invalidateDenseIndexToSparseIndex(currentSize);
			removeDenseValueAtCurrentSize(currentSize);
		}
		void invalidateSparseIndex(int sparseIndex) {
			throwIfInvalidSparseIndex(sparseIndex);
			sparseIndices[sparseIndex] = invalidIndex;
		}
		void invalidateDenseIndexToSparseIndex(int denseIndex) {
			denseIndexToSparseIndex[denseIndex] = invalidIndex;
		}
		void removeDenseValueAtCurrentSize() {
			if (currentSize != denseValues.size() - 1) {
				throw std::exception("Trying to erase element that isn't the last!");
			}
			//vector.erase will move all subsequent elements
			//therefore we only allow for erasing the final element
			denseValues.erase(denseValues.begin() + currentSize);
		}

		void overwriteWithElementAtCurrentSize(int denseIndex) {
			//A = element to overwrite, 
			//B = element at current size (last element)
			
			//move the value of B to A
			denseValues[denseIndex] = denseValues[currentSize];
			//erase old B
			removeDenseValueAtCurrentSize();

			int sparseIndexA = denseIndexToSparseIndex[denseIndex];
			int sparseIndexB = denseIndexToSparseIndex[currentSize];

			//make the sparse index entry for B point to the new position at A
			if (isValidSparseIndex(sparseIndexB)) {
				sparseIndices[sparseIndexB] = denseIndex;
			}
			//invalidate the sparse index entry for A
			if (isValidSparseIndex(sparseIndexA)) {
				invalidateSparseIndex(sparseIndexA);
			}

			//make the dense index entry for the new B point to the new sparse index entry for B
			denseIndexToSparseIndex[denseIndex] = sparseIndexB;
			//invalidate the old value to index 
			invalidateDenseIndexToSparseIndex(currentSize);
		}

		void appendToBack(int sparseIndex, T value) {
			sparseIndices[sparseIndex] = currentSize;
			denseValues[currentSize] = value;
			denseIndexToSparseIndex[currentSize] = sparseIndex;
			++currentSize;
		}

		//throw functions
		void throwIfInvalidSparseIndex(int sparseIndex) {
			if (isInvalidSparseIndex(sparseIndex)) {
				throw std::exception("Invalid sparse index: " + std::to_string(sparseIndex));
			}
		}
		void throwIfInvalidDenseIndex(int denseIndex) {
			if (isInvalidDenseIndex(denseIndex)) {
				throw std::exception("Invalid dense index: " + std::to_string(denseIndex));
			}
		}
	};
}