#pragma once

#include "KeyState.h"

#include "KeyValues.h"

//the interface exposed to the game i.e. getting state and ending turn
namespace wasp::input {
	class IKeyInputTable {
	public:
		virtual KeyState operator[](KeyValues key) = 0;
		virtual KeyState get(KeyValues key) = 0;
		virtual void turnOver() = 0;
	};
}