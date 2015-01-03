#include "WPILib.h"

class Catapult
{
protected:
	Victor* winchMotor;
	Victor* cageMotor;
	DigitalInput* cageSwitchOut;
	DigitalInput* cageSwitchIn;
	Timer* timador;
	DigitalInput* limitSwitch; //Pass this off, after constructed, to DSLCD
	
	
	int rollWinchInvert;
	
public:
	Catapult(int winchMotorChannel, int cageMotorChannel, int lowerLimitChannel, int cageInChannel, int cageOutChannel);
	void TestWinch(float rollSpeed);
	void SlideCage(bool outgoing);
	void CageTest(float cageSpeed);
	void CageOut();
	void Message(DriverStationLCD * dslcd, bool rollWinch, int catSwitchLine);
	int LimitSwitch();
};
