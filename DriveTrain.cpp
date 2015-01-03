#include "DriveTrain.h"
#include "Defines.h"
#include "Math.h"

DriveTrain::DriveTrain(int leftMotor, int rightMotor)
{
	drive = new RobotDrive(leftMotor, rightMotor);
	leftHandicapSubtraction = 0;
	rightHandicapSubtraction = 0;
	messageUse = 0;
	handicapHasBeenPressed = false;
}

void DriveTrain::TankDrive(GenericHID * leftJoystick, GenericHID * rightJoystick)
{
	float leftSpeed = leftJoystick->GetY() * LEFT_MOTOR_INVERT;
	float rightSpeed = rightJoystick->GetY() * RIGHT_MOTOR_INVERT;
	float leftHandicap;
	float rightHandicap;
	
	if(leftJoystick->GetRawButton(LR_BUTTON_TURBO) || rightJoystick->GetRawButton(LR_BUTTON_TURBO))
	{
		rightSpeed *= TURBO_SPEED;
		leftSpeed *= TURBO_SPEED;
	}
	else
	{
		rightSpeed *= NORMAL_SPEED;
		leftSpeed *= NORMAL_SPEED;
	}
	
	if(leftJoystick->GetRawButton(L_RIGHT_SUBTRACT))
	{
		if(handicapHasBeenPressed==false)
		{
			rightHandicapSubtraction += .05;
		}
		handicapHasBeenPressed = true;
	}
	else if(leftJoystick->GetRawButton(L_LEFT_SUBTRACT))
	{
		if(handicapHasBeenPressed==false)
		{
			leftHandicapSubtraction += .05;
		}
		handicapHasBeenPressed = true;
	}
	else
	{
		handicapHasBeenPressed = false;
	}
	
	if(leftJoystick->GetRawButton(L_SUBTRACT_RESET))
	{
		leftHandicapSubtraction = 0;
		rightHandicapSubtraction = 0;
	}
	
	leftHandicap = 1 - leftHandicapSubtraction;
	rightHandicap = 1 - rightHandicapSubtraction;
	
	if(rightSpeed > .9) // Only works when going forward
	{
		rightSpeed *= rightHandicap;
	}
	
	if(leftSpeed > .9)
	{
		leftSpeed *= leftHandicap;
	}
	
	drive -> TankDrive(leftSpeed, rightSpeed);
}


void DriveTrain::XBoxDrive(GenericHID * XStick)
{
	float forward = XStick->GetRawAxis(2); //2 is a controller's LeftY
	float turn = XStick->GetRawAxis(4); //4 is a controller's RightX
	float turbo = XStick->GetRawAxis(3); //3 seems to be either Trigger, but will only work if use Left Trigger, which
	float leftHandicap;					//controls the axis from 0-1
	float rightHandicap;

	
	if(turbo <= 0)
	{
		turbo = 0;
	}
	else
	{
		turbo = turbo / 2; //So output will range from 0 to .5, to add to minimum speed
	}
		
	if(fabs(forward) < .0125)	//To buffer; XStick is sensitive
	{
		forward = 0;
	}
		
	if(fabs(turn) < .0125)
	{
		turn = 0;
	}
	
	if(turbo < .0125)
	{
		turbo = 0;
	}
	
	if(XStick->GetRawButton(X_RIGHT_BUMPER))
	{
		turn *= .5;
	}
	
	if(forward < 0)
	{
		turn *= -1;  // Turns the same direction if going forward or backward
	}
	
	forward *= (.5 + turbo);	//Modify speed with turbo, minimum of .5
	
	if(XStick->GetRawButton(X_LEFT_CLICK) && XStick->GetRawButton(X_RIGHT_CLICK) && XStick->GetRawButton(X_RIGHT_BUMPER))
	{
		if(handicapHasBeenPressed==false)
		{
			rightHandicapSubtraction += .05;
		}
		handicapHasBeenPressed = true;
	}
	else if(XStick->GetRawButton(X_LEFT_CLICK) && XStick->GetRawButton(X_RIGHT_CLICK) && XStick->GetRawButton(X_LEFT_BUMPER))
	{
		if(handicapHasBeenPressed==false)
		{
			leftHandicapSubtraction += .05;
		}
		handicapHasBeenPressed = true;
	}
	else
	{
		handicapHasBeenPressed = false;
	}
	
	if(XStick->GetRawButton(X_BACK) && XStick->GetRawButton(X_START))
	{
		leftHandicapSubtraction = 0;
		rightHandicapSubtraction = 0;
	}
	
	leftHandicap = 1 - leftHandicapSubtraction;
	rightHandicap = 1 - rightHandicapSubtraction;
	
	forward *= DRIVE_INVERT_FORWARD;
	turn *= DRIVE_INVERT_TURN;
	
	DissectedDrive(forward, turn, leftHandicap, rightHandicap);
}

void DriveTrain::Drive(float forward, float turn)
{
	forward *= DRIVE_INVERT_FORWARD;
	turn *= DRIVE_INVERT_TURN;
	drive->Drive(forward, turn);
}

void DriveTrain::DissectedDrive(float speed, float curve, float leftHandicap, float rightHandicap)
{		//Speed and curve are the same as forward and turn, respectively
	float leftSpeed, rightSpeed;
	float m_sensitivity = .5;

	if (curve < 0)
	{
		float value = log(-curve);
		float ratio = (value - m_sensitivity)/(value + m_sensitivity);
		if (ratio == 0) ratio =.0000000001;
			leftSpeed = speed / ratio;
			rightSpeed = speed;
	}
	else if (curve > 0)
	{
		float value = log(curve);
		float ratio = (value - m_sensitivity)/(value + m_sensitivity);
		if (ratio == 0) ratio =.0000000001;
			leftSpeed = speed;
			rightSpeed = speed / ratio;
	}
	else
	{
		leftSpeed = speed;
		rightSpeed = speed;
	}
	
	if (speed > .9) // Only works when going forward
	{
		leftSpeed *= leftHandicap;
		rightSpeed *= rightHandicap;
	}
	drive->SetLeftRightMotorOutputs(leftSpeed, rightSpeed);
}

float DriveTrain::Handicaps(bool right)
{
	float value;
	if(right == true)
	{
		value = rightHandicapSubtraction;
	}
	else if(right == false)
	{
		value = leftHandicapSubtraction;
	}
	return value;
}

void DriveTrain::Message(DriverStationLCD * dslcd)
{
	if(messageUse != (2 - rightHandicapSubtraction - leftHandicapSubtraction) / 2 ) // Just so that if either variable
	{																			  // changes we'll rerun the thing
	float rightHandicap = 1 - rightHandicapSubtraction;
	float leftHandicap = 1 - leftHandicapSubtraction;
	dslcd->PrintfLine(dslcd->kUser_Line5,"R Turbo: %f",rightHandicap);
	dslcd->PrintfLine(dslcd->kUser_Line6,"L Turbo: %f",leftHandicap);
	
	messageUse = (2 - rightHandicapSubtraction - leftHandicapSubtraction) / 2;
	dslcd->UpdateLCD();
	}
}
