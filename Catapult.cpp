#include "Catapult.h"
#include "Defines.h"
#include "Pneumatics.h"
#include "Math.h"

Catapult::Catapult(int winchMotorChannel, int cageMotorChannel, int lowerLimitChannel, int cageInChannel, int cageOutChannel)
{
	winchMotor = new Victor(winchMotorChannel);
	cageMotor = new Victor(cageMotorChannel);
	limitSwitch = new DigitalInput(lowerLimitChannel);
	cageSwitchIn = new DigitalInput(cageInChannel);
	cageSwitchOut = new DigitalInput(cageOutChannel);
	timador = new Timer();
	
	
	rollWinchInvert = -1;
}

void Catapult::TestWinch(float rollSpeed)
{
	if(limitSwitch->Get() == 0)
	{
		rollSpeed *= TEST_ROLL_SPEED;
		// rollSpeed = -1 * fabs(rollSpeed);  // We only want it to go in one direction
		winchMotor->Set(rollSpeed);
	}
	else
	{
		if(rollSpeed != 0)
		{winchMotor->Set(.1);}
	}
}

int Catapult::LimitSwitch()
{
	int value;
	value = limitSwitch->Get();
	return value; // 1 is unpressed, 0 is pressed
}

void Catapult::SlideCage(bool outgoing)
{
	float speed = .5;
	speed *= CAGE_SLIDE_INVERT;
	
	if(outgoing)
	{
		while(cageSwitchOut->Get() == 1)
		{
			cageMotor->Set(speed);
		}
		
		cageMotor->Set(0);
		
	}
	else
	{
		speed *= -1;
		
		while(cageSwitchIn->Get() == 1)
		{
		cageMotor->Set(speed);
		}
		
		cageMotor->Set(0);
	}
	
}

void Catapult::CageTest(float speed)
{
	cageMotor->Set(speed);
}


void Catapult::Message(DriverStationLCD * dslcd, bool rollWinch, int catSwitchLine)
{
	DriverStationLCD::Line line;
	
	if(catSwitchLine = 5)
	{line = DriverStationLCD::kUser_Line5;}
	else if(catSwitchLine = 6)
	{line = DriverStationLCD::kUser_Line6;}
	
	int limit = limitSwitch->Get();
	dslcd->PrintfLine(line,"Cat Switch: %i",limit);
	
	if(rollWinch)
	{
		dslcd->PrintfLine(dslcd->kUser_Line6,"Winch Rolling!!");
	}
	
	dslcd->UpdateLCD();
}
