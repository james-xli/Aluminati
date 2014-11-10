#pragma config(Hubs,  S1, HTMotor,  HTMotor,  none,     none)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Motor,  mtr_S1_C1_1,     FLWheel,       tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C1_2,     FRWheel,       tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C2_1,     BLWheel,       tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C2_2,     BRWheel,       tmotorTetrix, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "JoystickDriver.c"

// Constants
#define FORWARD	1
#define RIGHT	2
#define BACK	3
#define LEFT	4

#define ROTATE_LEFT		1
#define ROTATE_RIGHT	2

#define FULL_MOTOR_POWER 90

#define LEFT_ORIENTATION_HOLO_WHEEL		1
#define RIGHT_ORIENTATION_HOLO_WHEEL	2

#define LOOP_INTERVAL 5

#define DEADBAND(x) ((abs(x) >= 10)? x: 0)

float modAngle(float angle) {
	while (angle >= 360) {
		angle = angle - 360;
	}
	while (angle < 0) {
		angle = angle + 360;
	}
	return angle;
}

// Given a point within the Cartesian coordinate system, this function will calculate the angle, in standard position, from the origin to the given point.
float getAngle(float x, float y) {
	float angle	= 0;
	float refAngle	= 0;
	//int quadrant	= 0;

	/*
	// Find Quadrant
	if (x < 0) {
		if (y < 0) {
			quadrant = 3;
		} else if (y > 0) {
			quadrant = 2;
		}
	} else if (x > 0) {
		if (y < 0) {
			quadrant = 4;
		} else if (y > 0) {
			quadrant  = 1;
		}
	}
	*/

	// Find angle in Standard Position
	if (x != 0 && y != 0) { // Exclude any Quadrantal Angles
		// Find Reference Angle
		refAngle = atan(y/x)*180/PI;

		// Find Angle in Standard Position
		if (x < 0) { // Quadrant II or III
			angle = 180 + refAngle;
		} else if (x > 0) { // Quadrant IV or I
			angle = 360 + refAngle;
		}

		// Ensure angle is within bounds: [0, 360)
		angle = modAngle(angle);
	}
	// Old Algorithm
	/*
	if (x != 0 && y != 0) { // Exclude any Quadrantal Angles
		// Find Reference Angle
		refAngle = atan(y/x);

		// Find Angle in Standard Position
		if (x < 0) { // Quadrant II or III
			if (refAngle < 0) { // Quadrant II or IV therefore Quadrant II
				angle = 180 + refAngle;
			} else if (refAngle > 0) { // Quadrant I or III therefore Quadrant III
				angle = 180 + refAngle;
			}
		} else if (x > 0) { // Quadrant IV or I
			if (refAngle < 0) { // Quadrant II or IV therefore Quadrant IV
				angle = 360 + refAngle;
			} else if (refAngle > 0) { // Quadrant I or III therefore Quadrant I
				angle = refAngle;
			}
		}
	}
	*/

	// Quadrantal Angles
	if (x == 0) {
		if (y == 0) {
			angle = 0; // Default to 0 degrees if point == origin
		} else if (y < 0) {
			angle = 270;
		} else if (y > 0) {
			angle = 90;
		}
	} else if (y == 0) {
		if (x < 0) {
			angle = 180;
		} else if (x > 0) {
			angle = 0;
		}
	}

	// TODO
	//else {
  //	writeDebugStreamLine("ERROR: Invalid value for 'x'. Value must be an integer within the bounds: [-100,100].");
	//}
	return angle;
}

float getRefAngle(float angle) {
	float refAngle;
	if			(angle < 90  && angle > 0) {
		refAngle = angle;
	} else if	(angle < 180 && angle > 90) {
		refAngle = 180 - angle;
	} else if	(angle < 270 && angle > 180) {
		refAngle = angle - 180;
	} else if	(angle < 360 && angle > 270) {
		refAngle = 360 - angle;
	}

	// Quadrantal Angles
	if (angle == 0 || angle == 180) {
		refAngle = 0;
	} else if (angle == 90 || angle == 270) {
		refAngle = 90;
	}
	return refAngle;
}

float remapJoystickInput(float joy) { // Remaps Joystick input [-128, 127] to [-100, 100]
	return (joy/128)*100;
}

// Given a point within the Cartesian coordinate system, this function will calculate the motor power based upon the distance from the origin to the given point.
float getPower(float x, float y) {
	// x and y must be within [-100, 100]
	float power;
	power = sqrt((x*x) + (y*y));
	if (power > 100) {
		power = 100;
	} else if (power < -100) {
		power = -100;
	}
	return power/100;
}

float getMotorPower(float power, float direction, int wheelOri) {
	float finPower;
	float tempDir;

	if (wheelOri == LEFT_ORIENTATION_HOLO_WHEEL) {
		tempDir = modAngle(direction - 45);
	} else if (wheelOri == RIGHT_ORIENTATION_HOLO_WHEEL) {
		tempDir = modAngle(direction + 45);
	}

	finPower = getRefAngle(tempDir)/90*100;
	if (tempDir > 180) {
		finPower = -finPower;
	}

	finPower = finPower*power;

	return finPower;
}

void holoDrive(int joy1_x, int joy1_y, int joy2_x, int joy2_y) {
	// Joystick input must be within {-100 - 100}

	// Movement

	// Rotation
}

/*
void simpleHoloDrive(int movement, int rotation) {
	// Movement input must be: {1, 2, 3, 4} corresponding to {forward, right, back, left}
	// Rotation input must be: {1, 2} corresponding to {rotateLeft, rotateRight}
}
*/

void simpleHoloMove(int movement) {
	// Movement input must be: {1, 2, 3, 4} corresponding to {forward, right, back, left}
	switch (movement) {
		case FORWARD:
			motor[FLWheel] = 1*FULL_MOTOR_POWER;
			motor[FRWheel] = 1*FULL_MOTOR_POWER;
			motor[BLWheel] = 1*FULL_MOTOR_POWER;
			motor[BRWheel] = 1*FULL_MOTOR_POWER;
			break;
		case RIGHT:
			motor[FLWheel] = 1*FULL_MOTOR_POWER;
			motor[FRWheel] = -1*FULL_MOTOR_POWER;
			motor[BLWheel] = -1*FULL_MOTOR_POWER;
			motor[BRWheel] = 1*FULL_MOTOR_POWER;
			break;
		case BACK:
			motor[FLWheel] = -1*FULL_MOTOR_POWER;
			motor[FRWheel] = -1*FULL_MOTOR_POWER;
			motor[BLWheel] = -1*FULL_MOTOR_POWER;
			motor[BRWheel] = -1*FULL_MOTOR_POWER;
			break;
		case LEFT:
			motor[FLWheel] = -1*FULL_MOTOR_POWER;
			motor[FRWheel] = 1*FULL_MOTOR_POWER;
			motor[BLWheel] = 1*FULL_MOTOR_POWER;
			motor[BRWheel] = -1*FULL_MOTOR_POWER;
			break;
	}
}

void simpleHoloRotate(int rotation) {
	// Rotation input must be: {1, 2} corresponding to {rotateLeft, rotateRight}

}

task main() {
	while (true) {
		wait1Msec(LOOP_INTERVAL);
		getJoystickSettings(joystick);

		//writeDebugStreamLine("Looop");
		//writeDebugStreamLine("%d", joy1_x1);
		/*
		if (joystick.joy1_x1 > 50) {
			writeDebugStreamLine("JOY");
		}
		*/
		if (joy1Btn(4)) {
			writeDebugStreamLine("JOY4BUTTON");
		}

		// TEST CODE
		writeDebugStreamLine("%f", getMotorPower(getPower(remapJoystickInput(DEADBAND(joystick.joy1_x1)), remapJoystickInput(DEADBAND(joystick.joy1_y1))), getAngle(remapJoystickInput(DEADBAND(joystick.joy1_x1)), remapJoystickInput(DEADBAND(joystick.joy1_y1))), RIGHT_ORIENTATION_HOLO_WHEEL));
		//writeDebugStreamLine("%d", joystick.joy1_y1);
		//writeDebugStreamLine("%f", getRefAngle(getAngle(remapJoystickInput(DEADBAND(joystick.joy1_x1)), remapJoystickInput(DEADBAND(joystick.joy1_y1))));
		//writeDebugStreamLine("%f", getPower(remapJoystickInput(DEADBAND(joystick.joy1_x1)), remapJoystickInput(DEADBAND(joystick.joy1_y1))));
		//writeDebugStreamLine("%f", getAngle(remapJoystickInput(joystick.joy1_x1), remapJoystickInput(joystick.joy1_y1)));
	}
}
