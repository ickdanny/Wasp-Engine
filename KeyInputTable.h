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
		static constexpr dataType thisTurn{ 1 << 0 }; // 0001
		static constexpr dataType lastTurn{ 1 << 1 }; // 0010

		static constexpr int numKeys{ static_cast<int>(KeyValues::numKeys) };

		std::array<KeyState, numKeys> stateArray{};
		std::array<dataType, numKeys> dataArray{};

	public:

		void handleKeyDown(WPARAM wParam, LPARAM lParam) override;

		void handleKeyUp(WPARAM wParam, LPARAM lParam) override;

		void allKeysOff() override;

		KeyState operator[](KeyValues key) override;

		KeyState get(KeyValues key) override;

		void turnOver() override;

	};
}