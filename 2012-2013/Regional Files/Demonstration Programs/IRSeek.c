#pragma config(Hubs,  S1, HTServo,  HTMotor,  HTMotor,  none)
#pragma config(Sensor, S2,     sonar,          sensorSONAR)
#pragma config(Sensor, S3,     IRSeekA,        sensorNone)
#pragma config(Sensor, S4,     HTIRS2,         sensorI2CCustom)
#pragma config(Motor,  mtr_S1_C2_1,     LeftBase,      tmotorTetrix, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C2_2,     RightBase,     tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C3_1,     Arm,           tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C3_2,     elev,          tmotorTetrix, openLoop, encoder)
#pragma config(Servo,  srvo_S1_C1_1,    servo1,               tServoStandard)
#pragma config(Servo,  srvo_S1_C1_2,    servo2,               tServoNone)
#pragma config(Servo,  srvo_S1_C1_3,    servo3,               tServoNone)
#pragma config(Servo,  srvo_S1_C1_4,    servo4,               tServoNone)
#pragma config(Servo,  srvo_S1_C1_5,    servo5,               tServoNone)
#pragma config(Servo,  srvo_S1_C1_6,    servo6,               tServoNone)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

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

#pragma fileExtension("rtm")
#pragma platform("NXT")

#include "hitechnic-irseeker-v2.h"
#include "AdvancedSensors.c"
// main task
int _dirAC = 0;
int dist = 200;

typedef enum
{
	sonarNoSensorConnected = 254,
	sonarOutOfRange        = 255
} TSonarErrorCodes;


#pragma platform (NXT, FTC)
#pragma fileExtension("rtm")


task main ()
{
_dirAC = 0;
int acS1, acS2, acS3, acS4, acS5 = 0;
int maxSig = 0; // the max signal strength from the seeker.
int val = 0; // the translated directional value.
// we are going to set DSP mode to 1200 Hz.
tHTIRS2DSPMode _mode = DSP_1200;
// attempt to set to DSP mode.
if (HTIRS2setDSPMode(HTIRS2, _mode) == 0)
{
// unsuccessful at setting the mode.
// display error message.
eraseDisplay();
nxtDisplayCenteredTextLine(0, "ERROR!");
nxtDisplayCenteredTextLine(2, "Init failed!");
nxtDisplayCenteredTextLine(3, "Connect sensor");
nxtDisplayCenteredTextLine(4, "to Port 1.");
// make a noise to get their attention.
PlaySound(soundBeepBeep);
// wait so user can read message, then leave main task.
wait10Msec(300);
return;
}
eraseDisplay();
// loop continuously and read from the sensor.
while(true)
{
// read the current modulated signal direction
_dirAC = HTIRS2readACDir(HTIRS2);
if (_dirAC < 0)
{
// error! - write to debug stream and then break.
writeDebugStreamLine("Read dir ERROR!");
break;
}
// Get the AC signal strength values.
if (!HTIRS2readAllACStrength(HTIRS2, acS1, acS2, acS3, acS4, acS5 ))
{
// error! - write to debug stream and then break.
writeDebugStreamLine("Read sig ERROR!");
break;
} else {
// find the max signal strength of all detectors.
maxSig = (acS1 > acS2) ? acS1 : acS2;
maxSig = (maxSig > acS3) ? maxSig : acS3;
maxSig = (maxSig > acS4) ? maxSig : acS4;
maxSig = (maxSig > acS5) ? maxSig : acS5;
}
// display info
nxtDisplayCenteredBigTextLine(1, "Dir=%d", _dirAC);
nxtDisplayCenteredBigTextLine(4, "Sig=%d", maxSig);
// figure out which direction to go...
// a value of zero means the signal is not found.
// 1 corresponds to the far left (approx. 8 o'clock position).
// 5 corresponds to straight ahead.
// 9 corresponds to far right.
// first translate directional index so 0 is straight ahead.
val = _dirAC - 5;
// calculate left and right motor speeds.

dist = SensorValue[sonar];

if(val > 0){
	motor[LeftBase] = 60;
	motor[RightBase] = -60;
}
else if (val < 0){
	motor[LeftBase] = -60;
	motor[RightBase] = 60;
}
else if (dist > 80){
	motor[LeftBase] = 60;
	motor[RightBase] = 60;
}
else{
	motor[LeftBase] = 0;
	motor[RightBase] = 0;
}

// wait a little before resuming.
wait10Msec(2);
}
}