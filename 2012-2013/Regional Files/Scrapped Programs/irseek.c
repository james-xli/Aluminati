#pragma config(Sensor, S1, HTIRS2, sensorI2CCustom)
#pragma config(Sensor, S2, HTIRS2B, sensorI2CCustom)
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
// main task
task main ()
{
	int _dirAC = 0;
	int acS1, acS2, acS3, acS4, acS5 = 0;
	int maxSig = 0; // the max signal strength from the seeker.
	int val = 0; // the translated directional value.
	// we are going to set DSP mode to 1200 Hz.

	int _dirACB = 0;
	int acS1B, acS2B, acS3B, acS4B, acS5B = 0;
	int maxSigB = 0; // the max signal strength from the seeker.
	int valB = 0;

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

	if (HTIRS2setDSPMode(HTIRS2B, _mode) == 0)
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
		_dirACB= HTIRS2readACDir(HTIRS2B);
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

		// display info
		nxtDisplayCenteredTextLine(1, "Dir=%d", _dirAC);
		nxtDisplayCenteredTextLine(2, "Sig=%d", maxSig);
		nxtDisplayCenteredTextLine(3, "Dir=%d", _dirACB);
		nxtDisplayCenteredTextLine(4, "Sig=%d", maxSigB);
		wait10Msec(2);
	}
}
