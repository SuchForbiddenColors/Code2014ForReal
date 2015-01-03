#include "WPILib.h"
class Pneumatics
{
protected:
	Compressor *compress;
	Solenoid *solen1;
	Solenoid *solen2;
	Solenoid *solen3;
	Solenoid *solen4;
	Relay *spike;
	DigitalInput *reedSwitch;
	
	char* message;
	bool piston1Extended;
	bool piston2Extended;
	int connected;
	
public:
	Pneumatics(int pressureSwitchChannel, int compressorRelayChannel, int solenoidChannel1, int solenoidChannel2,
			int solenoidChannel3,int solenoidChannel4, int reedSwitchChannel);
	void Extend(int pistonNumber);
	void Retract(int pistonNumber);
	void Move(int pistonNumber);
	void Flash(bool connect);
	void Spike(bool forward);
	void Message(DriverStationLCD * dslcd);
	int PressureSwitch();
	int WinchConnect();
};
