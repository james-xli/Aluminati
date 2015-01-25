#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTServo,  HTMotor)
#pragma config(Sensor, S2,     bigBall,        sensorNone)
#pragma config(Sensor, S3,     smallBall,      sensorNone)
#pragma config(Sensor, S4,     ultrasonic,     sensorNone)
#pragma config(Motor,  mtr_S1_C1_1,     BLWheelMotor,  tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C1_2,     FLWheelMotor,  tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C2_1,     BRWheelMotor,  tmotorTetrix, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C2_2,     FRWheelMotor,  tmotorTetrix, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C4_1,     motor1,        tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C4_2,     winchMotor,    tmotorTetrix, openLoop)
#pragma config(Servo,  srvo_S1_C3_1,    leftHook,             tServoStandard)
#pragma config(Servo,  srvo_S1_C3_2,    rightHook,            tServoStandard)
#pragma config(Servo,  srvo_S1_C3_3,    servo3,               tServoNone)
#pragma config(Servo,  srvo_S1_C3_4,    servo4,               tServoNone)
#pragma config(Servo,  srvo_S1_C3_5,    servo5,               tServoNone)
#pragma config(Servo,  srvo_S1_C3_6,    servo6,               tServoNone)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "JoystickDriver.c"

void initializeRobot()
{

}

int deadband(int x)
{
	if (x <= 10 && x >= -10)
		x = 0;
	return x;
}

int motorScale(float x)
{
	x = ((x / 128) * 100);
	if (x > 100)
		x = 100;
	return x;
}

void drive(int x, int y, int r)
{
	float k = 1.0;
	float kR = 1.0;

	int xPower, yPower, rPower, FLPower, BLPower, BRPower, FRPower;

	xPower = k 	* motorScale(x);
	yPower = k 	* motorScale(y);
	rPower = kR	* motorScale(r);

	FLPower = motorScale(		xPower	+	yPower	+	rPower);
	BLPower = motorScale( -	xPower 	+	yPower	+	rPower);
	BRPower = motorScale(		xPower	+	yPower	-	rPower);
	FRPower = motorScale( -	xPower	+	yPower	-	rPower);

	motor[FLWheelMotor] = FLPower;
	motor[BLWheelMotor] = BLPower;
	motor[BRWheelMotor] = BRPower;
	motor[FRWheelMotor] = FRPower;
}

void servoDown ()
{
	int hookUpRight = 53;	// smaller value is more down, default was 31
	int hookUp = 248 - hookUpRight;

	servo[leftHook] = hookUp;
	servo[rightHook] = hookUpRight;
}

void servoUp ()
{
	int HookDownRight = 250; // greater value is more up
	int HookDown = 244 - HookDownRight;

	servo[leftHook] = HookDown;
	servo[rightHook] = HookDownRight;
}

task main()
{
	//initializeRobot();
	waitForStart();
	drive(0, 100, 0);		// drive forward off ramp
	servoUp();					// talons up
	wait1Msec(2950);
	servoDown();				// talons down
	wait1Msec(450);
	drive(-50, -35, 75);// spin around goal
	wait1Msec(1250);
	drive(75, 20, 75);	// spin even harder
	wait1Msec(1200);
	drive(50, 100, 40);	// spin less drive more
	wait1Msec(3000);
	drive(0, 100, 0);		// drive those last couple feet
	wait1Msec(650);
}
