#pragma once

#include <vector>

#include "Utility/Ticker.h"

namespace wasp::game::components {

	struct Animation {
		//fields
		std::vector<std::wstring> frames{};
		bool looping{};
		std::size_t currentIndex{};

		Animation(const std::vector<std::wstring>& frames, bool looping = true)
			: frames{ frames }
			, looping{ looping }
			, currentIndex{ 0 } {
		}

		std::wstring getCurrentFrame() {
			if (currentIndex >= frames.size()) {
				throw std::runtime_error{ "bad index in animation" };
			}
			return frames[currentIndex];
		}
	};

	struct AnimationList {
	private:
		//typedefs
		using Ticker = utility::Ticker;

	public:
		//fields
		std::vector<Animation> animations{};
		std::size_t idleIndex{};
		std::size_t currentIndex{};
		Ticker ticker;			//uninitialized!

		AnimationList(
			const std::vector<Animation>& animations, 
			std::size_t idleIndex, 
			std::size_t ticks
		)
			: animations{ animations }
			, idleIndex{ idleIndex }
			, currentIndex{ idleIndex }
			, ticker{ ticks, true } {
		}

		AnimationList(const Animation& animation, std::size_t ticks)
			: animations{ { animation } }
			, idleIndex{ 0 }
			, currentIndex{ idleIndex }
			, ticker{ ticks, true } {
		}

		AnimationList(const std::vector<std::wstring>& frames, std::size_t ticks)
			: animations{ {frames} }
			, idleIndex{ 0 }
			, currentIndex{ idleIndex }
			, ticker{ ticks, true } {
		}

		Animation& getCurrentAnimation() {
			if (currentIndex >= animations.size()) {
				throw std::runtime_error{ "bad index in animation list" };
			}
			return animations[currentIndex];
		}
	};
}