#pragma once

namespace wasp::utility {

	template <typename T>
	class TwoFrame : public T {

	private:
		//fields
		T past{ *this };

	public:
		//inherit constructors
		using T::T;

		void step() {
			past = *this;
		}

		T& getPast() {
			return past;
		}

		const T& getPast() const {
			return past;
		}
	};
}