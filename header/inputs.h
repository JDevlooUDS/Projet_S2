#pragma once

struct Inputs {

	bool isLeftPressed = false;
	bool isRightPressed = false;
	bool isEscapePressed = false;
	bool isSpacePressed = false;
	bool isDashPressed = false;


	void reset() {
		isLeftPressed = false;
		isRightPressed = false;
		isEscapePressed = false;
		isSpacePressed = false;
		isDashPressed = false;
	}
};