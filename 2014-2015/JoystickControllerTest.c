
#include "JoystickDriver.c"

// Constants
#define WAIT_TIME		5 // Milliseconds

task main() {
	while (true) {
		//writeDebugStreamLine("Loop");
		getJoystickSettings(joystick); // Update joystick
		if (joy1Btn(4)) {
			writeDebugStreamLine("Button 4 Pressed");
			PlayTone(659,   34);
		}
		if (joy1Btn(2)) {
			writeDebugStreamLine("Button 2 Pressed");
		}
		//writeDebugStreamLine("%d", joystick.joy1_y1);

		//writeDebugStreamLine("%d", joy1Btn(4));

		//writeDebugStreamLine("%d", joystick.joy1_Buttons);

		wait1Msec(WAIT_TIME);
	}
}
