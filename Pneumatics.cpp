#include "WPILib.h"
#include "Pneumatics.h"
#include "Defines.h"

Pneumatics::Pneumatics(int pressureSwitchChannel, int compressorRelayChannel, int solenoidChannel1, int solenoidChannel2,
		int solenoidChannel3, int solenoidChannel4, int reedSwitchChannel)
{
	compress = new Compressor(pressureSwitchChannel, compressorRelayChannel);
	compress->Start();
	
	solen1 = new Solenoid(solenoidChannel1);
	solen2 = new Solenoid(solenoidChannel2);
	solen3 = new Solenoid(solenoidChannel3);
	solen4 = new Solenoid(solenoidChannel4);
	
	reedSwitch = new DigitalInput(reedSwitchChannel);
		
	spike = new Relay(compressorRelayChannel);
	
	Retract(1);  // So piston stays in during construction
	
	piston1Extended = false;
	piston2Extended = false;
	connected = 2;
	

}

void Pneumatics::Spike(bool forward)
{
	Relay::Value kForward;
	Relay::Value kReverse;
	if(forward)
	{spike->Set(kForward);}
	else
	{spike->Set(kReverse);}
}

void Pneumatics::Extend(int pistonNumber)
{
if(pistonNumber == 1) // Big Piston
{
	solen1->Set(false);
	solen2->Set(true);
	piston1Extended = true;
}
else if(pistonNumber == 2) // Little Piston
{
	solen3->Set(false);
	solen4->Set(true);
	piston2Extended = true;
}
else
{
	message = "Error: solenoids unavailable";
}

}

void Pneumatics::Retract(int pistonNumber)
{
if(pistonNumber == 1)
{
	solen1->Set(true);
	solen2->Set(false);
	piston1Extended = false;
}
else if(pistonNumber == 2 && false) // 2 isn't a piston
{
	solen3->Set(true);
	solen4->Set(false);
	piston2Extended = false;
}
else
{
	message = "Error: solenoids unavailable";
}


}

void Pneumatics::Move(int pistonNumber)
{
	if(pistonNumber == 1)
	{
		if(piston1Extended == false)
		{
			Extend(1);
		}
		else
		{
			Retract(1);
		}
	}
	
	if(pistonNumber == 2 && false) // 2 isn't a piston
	{
		if(piston2Extended)
		{
			Retract(2);
		}
		else
		{
			Extend(2);
		}
	}
}

void Pneumatics::Flash(bool connect) // Quikcly flashes a bit of air to use ball shifter on catapult
{
	if(connect) // Using one solenoid will connect gearbox, one will disconnect
	{			
		solen4->Set(true); // 4 should connect motor to winch
		Wait(.5);
		connected = 1;
	}
	else
	{
		solen3->Set(true);
		Wait(.5);
		connected = 0;
	}
	solen3->Set(false);
	solen4->Set(false);
}

int Pneumatics::PressureSwitch()
{
	int value;
	value = compress->GetPressureSwitchValue();
	return value; // 1 is full
}

int Pneumatics::WinchConnect()
{
	return connected;
}

void Pneumatics::Message(DriverStationLCD * dslcd)
{
	int pressure = compress->GetPressureSwitchValue();
	char* press;
	char* connectMessage;
	int reed = reedSwitch->Get();
	if(pressure == 0)
	{
		press = "Not Full";
	}
	
	if(pressure == 1)
	{
		press = "Full";
	}
	
	/** char* piston1;
	if(piston1Extended)
	{
		piston1 = "Piston 1 out";
	}
	else
	{
		piston1 = "Piston 1 in";
	} **/
	
	if(connected==0)
	{connectMessage = "Not connected";}
	if(connected==1)
	{connectMessage = "Connected";}
	if(connected==2)
	{connectMessage = "Connect!";}
	
	
	dslcd->PrintfLine(dslcd->kUser_Line3,"%s",connectMessage);
	dslcd->PrintfLine(dslcd->kUser_Line4,"Pressure Switch: %s %i",press,pressure);
	
	dslcd->PrintfLine(dslcd->kUser_Line1,"%i",reed);
	
	dslcd->UpdateLCD();
}
