#include "JoystickDriver.c"

int hat_val = -1;

task main(){
	while(true){

		getJoystickSettings(joystick);
		hat_val = joystick.joy2_TopHat;
		nxtDisplayCenteredBigTextLine(3, "Hat=%d", hat_val);

	}

}
