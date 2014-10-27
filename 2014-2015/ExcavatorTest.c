#pragma config(Hubs,  S1, HTServo,  none,     none,     none)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Servo,  srvo_S1_C1_1,    excavator,            tServoStandard)
#pragma config(Servo,  srvo_S1_C1_2,    servo2,               tServoNone)
#pragma config(Servo,  srvo_S1_C1_3,    servo3,               tServoNone)
#pragma config(Servo,  srvo_S1_C1_4,    servo4,               tServoNone)
#pragma config(Servo,  srvo_S1_C1_5,    servo5,               tServoNone)
#pragma config(Servo,  srvo_S1_C1_6,    servo6,               tServoNone)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "JoystickDriver.c"
#define TIME_INTERVAL  1000


#define BOUND_SERVOIN(x) (((x) > 100)? 100: ((x) < -100)? -100: (x)) //*!!Makes sure that the joystick is between -100 and 100 !!*/
#define DEADBAND_SERVOIN(x) ((abs(x) >= 50)? x: 0)
/*Servo value of 0 = Full Power/Speed Reverse
Servo value of 127 = Stop
Servo value of 256 = Full Power/Speed Forward*/
int servo_in = 0;
int servo_out = 256;
int servo_stop = 128;


void claw_servo_in (int btn_x)
{
	//writeDebugStreamLine("%d", btn_x);
		if (btn_x == 1)
	{
		servo[excavator] = servo_in;
	}
}

void claw_servo_out (int btn_x)
{
		if (btn_x == 1)
	{
		servo[excavator] = servo_out;
	}
}

void claw_servo_stop (int btn_x)
{
		if (btn_x == 1)
	{
		servo[excavator] = servo_stop;
	}
}


task main()
{
	while(true)
	{

		getJoystickSettings(joystick);
		//servo[excavator] = servo_in;
		//wait1Msec(TIME_INTERVAL);
		//servo[excavator] = servo_out;
		//writeDebugStreamLine("%d", joy1Btn(3));
		//writeDebugStreamLine("1");
		claw_servo_in(joy1Btn(4));
		claw_servo_out(joy1Btn(2));
		claw_servo_stop(joy1Btn(3));

		wait1Msec(TIME_INTERVAL);
	}
}