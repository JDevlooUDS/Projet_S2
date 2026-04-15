#pragma once

struct Inputs {

	bool isLeftPressed = false;
	bool isRightPressed = false;
	bool isEscapePressed = false;
	bool isSpacePressed = false;
	bool isDashPressed = false;
	bool isDashPressedThisFrame = false;
	bool isUpPressed = false;
	bool isDownPressed = false;
	bool isDebugPressed = false;
	bool isAccelerated = false;
	bool isSelectPressed = false;
	bool isPausePressed = false;
	float volume = 0.0f;
	bool muon = false;


	void reset() {
		isLeftPressed = false;
		isRightPressed = false;
		isEscapePressed = false;
		isSpacePressed = false;
		isDashPressed = false;
		isDashPressedThisFrame = false;
		isDebugPressed = false;
		isAccelerated = false;
		isSelectPressed = false;
		isPausePressed = false;
		volume = 0.0f;
		muon = false;
	}
};