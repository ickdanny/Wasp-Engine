#pragma once

#include <array>

#include "IKeyInputReceiver.h"
#include "IKeyInputTable.h"
#include "KeyValues.h"

namespace wasp::input {
	class KeyInputTable : public IKeyInputReceiver, public IKeyInputTable {
	private:
		using dataType = std::uint_fast8_t;

		//bitmasks
		static constexpr dataType thisTick{ 1 << 0 };				   // 0001
		static constexpr dataType lastTick{ 1 << 1 };				   // 0010
		static constexpr dataType lastTwoTicks{ thisTick | lastTick }; // 0011

		static constexpr int numKeys{ static_cast<int>(KeyValues::numKeys) };

		std::array<dataType, numKeys> dataArray{};

	public:

		void handleKeyDown(WPARAM wParam, LPARAM lParam) override;
		void handleKeyUp(WPARAM wParam, LPARAM lParam) override;
		void allKeysOff() override;

		const KeyState operator[](KeyValues key) override;
		const KeyState get(KeyValues key) override;
		void tickOver() override;

	private:

		static KeyState getKeyState(dataType data);
	};
}