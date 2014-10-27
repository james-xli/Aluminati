#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTServo,  none)
#pragma config(Sensor, S3, HTIRS2, sensorI2CCustom)
#pragma config(Sensor, S4, HTIRS2B, sensorI2CCustom)
#pragma config(Motor,  mtr_S1_C1_1,     RightBase,     tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C1_2,     LeftBase,      tmotorTetrix, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C2_1,     Arm,           tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C2_2,     Elev,          tmotorTetrix, openLoop)
#pragma config(Servo,  srvo_S1_C3_1,    servo1,               tServoContinuousRotation)
#pragma config(Servo,  srvo_S1_C3_2,    servo2,               tServoNone)
#pragma config(Servo,  srvo_S1_C3_3,    servo3,               tServoNone)
#pragma config(Servo,  srvo_S1_C3_4,    servo4,               tServoNone)
#pragma config(Servo,  srvo_S1_C3_5,    servo5,               tServoNone)
#pragma config(Servo,  srvo_S1_C3_6,    servo6,               tServoNone)

//
// RoboSeek.c - This program uses the HiTechnic IR Seeker V2
// sensor to follow an IR source. This program requires the
// HTIRS2-driver.h driver provided with RobotC.
//
// You can copy the "drivers" subdirectory and place it
// in the same directory as this source file. if you do,
// you can use the following #include statement to specify
// the path to the driver.
//#include "drivers/HTIRS2-driver.h"
// if you use the "drivers/HTIRS2-driver.h" line,
// then you should comment out the following line.
#include "hitechnic-irseeker-v2.h"

#define FINAL_INTENSITY 100



int _dirAC = 0;
int _dirACB = 0;

int acS1, acS2, acS3, acS4, acS5 = 0;
int maxSig = 0; // the max signal strength from the seeker.
int val = 0; // the translated directional value.
// we are going to set DSP mode to 1200 Hz.

int acS1B, acS2B, acS3B, acS4B, acS5B = 0;
int maxSigB = 0; // the max signal strength from the seeker.
int maxSigF = 0;

bool hasRing = true;


//MOVEMENT METHODS

void move(int x){
motor[LeftBase] = x;
motor[RightBase] = x;
wait1Msec(50);
}

void stopMotors(){
	motor[LeftBase] = 0;
	motor[RightBase] = 0;
	wait1Msec(50);
}

void moveForward(int sec){
	move(100);
	wait1Msec(sec*1000);
	stopMotors();
	wait1Msec(500);
}

void moveBackward(int sec){
	move(-100);
	wait1Msec(sec*1000);
	stopMotors();
	wait1Msec(100);
}

void pointTurn(float sec, int dir){
		motor[LeftBase] = 100*dir; //+1 is right turn
		motor[RightBase] = -100*dir;
		wait1Msec(sec*1000);
		stopMotors();
		wait1Msec(100);
}

void turnRight(int angle){
	float time = (angle * 0.0091);
		pointTurn(time, 1);
}

void turnLeft(int angle){
	float time = (angle * 0.0091);
	pointTurn(time, -1);
}

void initializeRobot(){
	nMotorEncoder[Elev] =0;
	turnRight(45);
	motor[Arm] = 40;
	wait1Msec(2000);
	motor[Arm] = 15;
}


// main task
task main ()
{
	initializeRobot();
	tHTIRS2DSPMode _mode = DSP_1200;
	// attempt to set to DSP mode.
	if (HTIRS2setDSPMode(HTIRS2, _mode) == 0)
	{
		// unsuccessful at setting the mode.
		// display error message.
		eraseDisplay();
		nxtDisplayCenteredTextLine(0, "ERROR!");
		nxtDisplayCenteredTextLine(2, "Unit failed!");
		nxtDisplayCenteredTextLine(3, "Connect sensor");
		nxtDisplayCenteredTextLine(4, "to Port 3.");
		// make a noise to get their attention.
		PlaySound(soundBeepBeep);
		// wait so user can read message, then leave main task.
		wait10Msec(300);
		return;
	}
	eraseDisplay();

	if (HTIRS2setDSPMode(HTIRS2B, _mode) == 0)
	{
		// unsuccessful at setting the mode.
		// display error message.
		eraseDisplay();
		nxtDisplayCenteredTextLine(0, "ERROR!");
		nxtDisplayCenteredTextLine(2, "Init failed!");
		nxtDisplayCenteredTextLine(3, "Connect sensor");
		nxtDisplayCenteredTextLine(4, "to Port 4.");
		// make a noise to get their attention.
		PlaySound(soundBeepBeep);
		// wait so user can read message, then leave main task.
		wait10Msec(300);
		return;
	}
	eraseDisplay();
	// loop continuously and read from the sensor.


	while((_dirAC + _dirACB) == 0){ //move until it finds the beacon
		_dirAC = HTIRS2readACDir(HTIRS2);
		_dirACB= HTIRS2readACDir(HTIRS2B);
		motor[LeftBase] = 50;
		motor[RightBase] = 50;
		wait10Msec(1);
	}
	motor[LeftBase] = 0;
	motor[RightBase] = 0;
	wait10Msec(1);

	while(hasRing)
	{
		// read the current modulated signal direction

		if (_dirAC < 0)
			{
		// error! - write to debug stream and then break.
		writeDebugStreamLine("Read dir ERROR!");
		break;
		}

		if (_dirACB < 0)
			{
		// error! - write to debug stream and then break.
		writeDebugStreamLine("Read dir ERROR!");
		break;
		}

		_dirAC = HTIRS2readACDir(HTIRS2);
		_dirACB= HTIRS2readACDir(HTIRS2B);
		val = 10 - _dirAC - _dirACB;

		// Get the AC signal strength values.
		if (!HTIRS2readAllACStrength(HTIRS2, acS1, acS2, acS3, acS4, acS5 ))
		{
		// error! - write to debug stream and then break.
		writeDebugStreamLine("Read sig ERROR!");
		break;
		}
		else {
		// find the max signal strength of all detectors.
		maxSig = (acS1 > acS2) ? acS1 : acS2;
		maxSig = (maxSig > acS3) ? maxSig : acS3;
		maxSig = (maxSig > acS4) ? maxSig : acS4;
		maxSig = (maxSig > acS5) ? maxSig : acS5;
		}

		if (!HTIRS2readAllACStrength(HTIRS2B, acS1B, acS2B, acS3B, acS4B, acS5B ))
		{
		// error! - write to debug stream and then break.
		writeDebugStreamLine("Read sig ERROR!");
		break;
		}
		else {
		// find the max signal strength of all detectors.
		maxSigB = (acS1B > acS2B) ? acS1B : acS2B;
		maxSigB = (maxSigB > acS3B) ? maxSigB : acS3B;
		maxSigB = (maxSigB > acS4B) ? maxSigB : acS4B;
		maxSigB = (maxSigB> acS5B) ? maxSigB : acS5B;
		}

		maxSigF = (maxSig > maxSigB)? maxSig: maxSigB;

		// display info
		nxtDisplayCenteredBigTextLine(1, "Sig=%d", maxSigF);
		nxtDisplayCenteredBigTextLine(5, "Dir=%d", val);

		if(maxSigF < 50){
			motor[LeftBase] = 40 + (val * 60);
			motor[RightBase] = 40 - (val * 60);
			wait1Msec(5);
		}

		else if(maxSigF < 60){
			motor[LeftBase] = 60 + (val * 60);
			motor[RightBase] = 60 - (val * 60);
			wait1Msec(5);
		}

		else if(maxSigF < 80){
			motor[LeftBase] = 30 + (val * 60);
			motor[RightBase] = 30 - (val * 60);
		}

		else{
			wait10Msec(30);
			stopMotors();
			motor[Arm] = -25;
			wait1Msec(2000);
			motor[Arm] = 10;
			moveBackward(1.5);
			turnRight(180);
			hasRing = false;
		}


		wait1Msec(10);
	}

	motor[LeftBase] = 0;
	motor[RightBase] = 0;

	wait1Msec(10);

}
