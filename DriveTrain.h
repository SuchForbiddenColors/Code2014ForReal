#include "WPILib.h"
class DriveTrain
{
protected:
	RobotDrive* drive;
	float leftHandicapSubtraction;
	float rightHandicapSubtraction;
	float messageUse;
	bool handicapHasBeenPressed;
	char* message;
public:
	
	DriveTrain(int leftMotor, int rightMotor);
	void TankDrive(GenericHID * leftJoystick, GenericHID * rightJoystick);
	void Drive(float forward, float turn);
	void DissectedDrive(float speed, float curve, float leftHandicap, float rightHandicap);
	void XBoxDrive(GenericHID * XStick);
	void Message(DriverStationLCD * dslcd);
	float Handicaps(bool right);
};
