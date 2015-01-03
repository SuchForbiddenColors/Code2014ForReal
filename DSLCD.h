#include "WPILib.h"
#include "Catapult.h"
#include "Defines.h"
#include "Pneumatics.h"
#include "DriveTrain.h"


class DSLCD
{
protected:
	int catLine;
	int winchRollLine;
	int rTurboLine;
	int lTurboLine;
	int connectMessageLine;
	int pressureSwitchLine;  //Need a number assignment system for these
	
	float subtractionMessageUse;
	
	int scrollNumber;
	
	int numSet [6];
	int numSet1 [6];
	int numSet2 [6];
	int currentSetNumber;
	int currentScrollNumber;
	bool resetSetNumber;

public:	
		Pneumatics *air;
		Catapult *cat;
		DriveTrain * car;
		DSLCD();
		void WriteMessage(DriverStationLCD * dsl, Catapult * cat, Pneumatics * air, DriveTrain * car,
						  bool rollWinch, int setNumber, int scrollNumber);
		DriverStationLCD::Line ConvertLine(int number, DriverStationLCD * dslcd);
			//TODO: Either this will give a kUser_Line value, or we need to 
		   //pass it off as a variable
		void AssignLine(int setNumber, int scrollNumber);

};

