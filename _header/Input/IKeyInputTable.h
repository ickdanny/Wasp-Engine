#pragma once

#include "KeyState.h"

#include "KeyValues.h"

//the interface exposed to the game i.e. getting state and ending turn
namespace wasp::input {
	class IKeyInputTable {
	public:
		virtual const KeyState operator[](KeyValues key) = 0;
		virtual const KeyState get(KeyValues key) = 0;
		virtual void tickOver() = 0;
	};
}