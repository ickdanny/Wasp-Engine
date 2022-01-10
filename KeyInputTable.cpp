#include "KeyInputTable.h"

namespace wasp::input {

	static KeyValues getKeyValue(WPARAM wParam);

	void wasp::input::KeyInputTable::handleKeyDown(WPARAM wParam, LPARAM lParam){
		dataArray[static_cast<int>(getKeyValue(wParam))] |= thisTick;
	}

	void wasp::input::KeyInputTable::handleKeyUp(WPARAM wParam, LPARAM lParam) {
		dataArray[static_cast<int>(getKeyValue(wParam))] &= ~thisTick;
	}

	void wasp::input::KeyInputTable::allKeysOff() {
		for (dataType& data : dataArray) {
			data &= ~thisTick;
		}
	}

	const KeyState KeyInputTable::operator[](KeyValues key){
		return get(key);
	}

	const KeyState KeyInputTable::get(KeyValues key) {
		return getKeyState(dataArray[static_cast<int>(key)]);
	}

	void KeyInputTable::tickOver() {
		for (dataType& data : dataArray) {
			data = (data << 1) | (data & thisTick); //shift left but repeat last digit
		}
	}

	KeyState KeyInputTable::getKeyState(dataType data) {
		switch (data & lastTwoTicks) {
			case 0:
			default:
				return KeyState::Up;
			case lastTwoTicks:
				return KeyState::Down;
			case 1:
				return KeyState::Press;
			case lastTick:
				return KeyState::Release;
		}
	}

	static KeyValues getKeyValue(WPARAM wParam) {
		switch(wParam) {
			//alphabet
			case 'A':
				return KeyValues::K_A;
			case 'B':
				return KeyValues::K_B;
			case 'C':
				return KeyValues::K_C;
			case 'D':
				return KeyValues::K_D;
			case 'E':
				return KeyValues::K_E;
			case 'F':
				return KeyValues::K_F;
			case 'G':
				return KeyValues::K_G;
			case 'H':
				return KeyValues::K_H;
			case 'I':
				return KeyValues::K_I;
			case 'J':
				return KeyValues::K_J;
			case 'K':
				return KeyValues::K_K;
			case 'L':
				return KeyValues::K_L;
			case 'M':
				return KeyValues::K_M;
			case 'N':
				return KeyValues::K_N;
			case 'O':
				return KeyValues::K_O;
			case 'P':
				return KeyValues::K_P;
			case 'Q':
				return KeyValues::K_Q;
			case 'R':
				return KeyValues::K_R;
			case 'S':
				return KeyValues::K_S;
			case 'T':
				return KeyValues::K_T;
			case 'U':
				return KeyValues::K_U;
			case 'V':
				return KeyValues::K_V;
			case 'W':
				return KeyValues::K_W;
			case 'X':
				return KeyValues::K_X;
			case 'Y':
				return KeyValues::K_Y;
			case 'Z':
				return KeyValues::K_Z;
			//numbers
			case '0':
				return KeyValues::K_0;
			case '1':
				return KeyValues::K_1;
			case '2':
				return KeyValues::K_2;
			case '3':
				return KeyValues::K_3;
			case '4':
				return KeyValues::K_4;
			case '5':
				return KeyValues::K_5;
			case '6':
				return KeyValues::K_6;
			case '7':
				return KeyValues::K_7;
			case '8':
				return KeyValues::K_8;
			case '9':
				return KeyValues::K_9;
			case VK_NUMPAD0:
				return KeyValues::K_numpad0;
			case VK_NUMPAD1:
				return KeyValues::K_numpad1;
			case VK_NUMPAD2:
				return KeyValues::K_numpad2;
			case VK_NUMPAD3:
				return KeyValues::K_numpad3;
			case VK_NUMPAD4:
				return KeyValues::K_numpad4;
			case VK_NUMPAD5:
				return KeyValues::K_numpad5;
			case VK_NUMPAD6:
				return KeyValues::K_numpad6;
			case VK_NUMPAD7:
				return KeyValues::K_numpad7;
			case VK_NUMPAD8:
				return KeyValues::K_numpad8;
			case VK_NUMPAD9:
				return KeyValues::K_numpad9;
			//arrow keys
			case VK_LEFT:
				return KeyValues::K_left;
			case VK_RIGHT:
				return KeyValues::K_right;
			case VK_UP:
				return KeyValues::K_up;
			case VK_DOWN:
				return KeyValues::K_down;
			//special
			case VK_ESCAPE:
				return KeyValues::K_escape;
			case VK_SPACE:
				return KeyValues::K_space;
			case VK_OEM_3:
				return KeyValues::K_backTick;
			case VK_OEM_MINUS:
				return KeyValues::K_minus;
			case VK_OEM_PLUS:
				return KeyValues::K_plus;
			case VK_OEM_5:
				return KeyValues::K_backSlash;
			case VK_BACK:
				return KeyValues::K_backSpace;
			case VK_RETURN:
				return KeyValues::K_enter;
			case VK_OEM_4:
				return KeyValues::K_openBracket;
			case VK_OEM_6:
				return KeyValues::K_closeBracket;
			case VK_OEM_1:
				return KeyValues::K_semicolon;
			case VK_OEM_7:
				return KeyValues::K_quote;
			case VK_OEM_COMMA:
				return KeyValues::K_comma;
			case VK_OEM_PERIOD:
				return KeyValues::K_period;
			case VK_OEM_2:
				return KeyValues::K_slash;
			case VK_TAB:
				return KeyValues::K_tab;
			case VK_SHIFT:
				return KeyValues::K_shift;
			case VK_CONTROL:
				return KeyValues::K_control;
			case VK_MENU:
				return KeyValues::K_alt;

			default:
				return KeyValues::undefined;
		}
	}
}