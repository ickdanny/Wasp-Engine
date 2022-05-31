#pragma once

#include "systemInclude.h"
#include "Input/IKeyInputTable.h"

namespace wasp::game::systems {

	class InputParserSystem {
	private:
		//typedefs
		using IKeyInputTable = input::IKeyInputTable;
		using KeyValues = input::KeyValues;
		using KeyState = input::KeyState;

		//fields
		IKeyInputTable* keyInputTablePointer{};

	public:
		InputParserSystem(IKeyInputTable* keyInputTablePointer)
			: keyInputTablePointer { keyInputTablePointer }{
		}

		void operator()(Scene& scene);

	private:
		//helper methods
		void parseMenuInput(Scene& scene);
		void parseGameInput(Scene& scene);

		bool isJustPressed(KeyValues keyValue);
	};
}