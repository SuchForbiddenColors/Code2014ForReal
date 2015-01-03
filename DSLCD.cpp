
DSLCD::DSLCD()
{
	
	numSet1[0] = 1;
	numSet1[1] = 2;
	numSet1[2] = 3;
	numSet1[3] = 4;
	numSet1[4] = 5;
	numSet1[5] = 6;
	
	numSet2[0] = 4;
	numSet2[1] = 2;
	numSet2[2] = 3;
	numSet2[3] = 6;
	numSet2[4] = 5;
	numSet2[5] = 1;
	
	currentSetNumber = 1;
	currentScrollNumber = 0;
	resetSetNumber = true;
	subtractionMessageUse = 0;
	
}

void DSLCD::WriteMessage(DriverStationLCD * dslcd, Catapult * cat, Pneumatics * air, DriveTrain * car,
						 bool rollWinch, int setNumber, int scrollNumber)
{						//Because this information comes from MyRobot
	
	AssignLine(setNumber, scrollNumber);
	
	int limit = cat->LimitSwitch();
		dslcd->PrintfLine(dslcd->kUser_Line6,"Cat Switch: %i",limit);
	
	int pressure = air->PressureSwitch();
		char* press;
		char* connectMessage;
		if(pressure == 0)
		{
			press = "Not Full";
		}
		
		if(pressure == 1)
		{
			press = "Full";
		}
		
		if(air->WinchConnect()==0)
		{connectMessage = "Not connected";}
		if(air->WinchConnect()==1)
		{connectMessage = "Connected";}
		if(air->WinchConnect()==2)
		{connectMessage = "Connect!";}
		
		float rightHandicap;
		float leftHandicap;
		
		if(subtractionMessageUse != (2 - car->Handicaps(true) - car->Handicaps(false)) / 2 ) // Just so that if either variable
			{																			  // changes we'll rerun the thing
			rightHandicap = 1 - car->Handicaps(true);
			leftHandicap = 1 - car->Handicaps(false);
			}
		subtractionMessageUse = (2 - car->Handicaps(true) - car->Handicaps(false)) / 2;
	
	dslcd->PrintfLine(ConvertLine(catLine, dslcd),"Cat Switch: %i",limit);
	if(rollWinch)
	{dslcd->PrintfLine(ConvertLine(winchRollLine, dslcd),"Winch Rolling!!");}
	dslcd->PrintfLine(ConvertLine(rTurboLine, dslcd),"R Turbo: %f",rightHandicap);
	dslcd->PrintfLine(ConvertLine(lTurboLine, dslcd),"L Turbo: %f",leftHandicap);
	dslcd->PrintfLine(ConvertLine(connectMessageLine, dslcd),"%s",connectMessage);
	dslcd->PrintfLine(ConvertLine(pressureSwitchLine, dslcd),"Pressure Switch: %s %i",press,pressure);
	dslcd->PrintfLine(dslcd->kUser_Line1, "This Line Is Intentionally Left Blank");
	
	
	dslcd->UpdateLCD();
	
}

DriverStationLCD::Line DSLCD::ConvertLine(int number, DriverStationLCD * dslcd)
{
	DriverStationLCD::Line line;
	
	if(number == 1)
	{
		line = dslcd->kUser_Line1;
	}
	else if(number == 2)
	{
		line = dslcd->kUser_Line2;
	}
	else if(number == 3)
	{
		line = dslcd->kUser_Line3;
	}
	else if(number == 4)
	{
		line = dslcd->kUser_Line4;
	}
	else if(number == 5)
	{
		line = dslcd->kUser_Line5;
	}
	else
	{
		line = dslcd->kUser_Line6;
	}
	
	return line;
}

void DSLCD::AssignLine(int setNumber, int scrollNumber)
{
	if(currentSetNumber != setNumber || currentScrollNumber != scrollNumber)
	{
		resetSetNumber = true;
	}
	
	if(resetSetNumber == true)
	{
		if(setNumber == 1)
		{
			int i = 0;
			while(i <= 5)
			{
				int h = i - scrollNumber;
				
				numSet[i] = numSet1[h];
			}
		}
		else if(setNumber == 2)
		{
			int j = 0;
			while(j <= 5)
			{
				int b = j - scrollNumber;
				
				numSet[j] = numSet2[b];
			}
		}
	
		catLine = numSet[0];
		winchRollLine = numSet[1];
		rTurboLine = numSet[2];
		lTurboLine = numSet[3];
		connectMessageLine = numSet[4];
		pressureSwitchLine = numSet[5];
	
	}
	
	currentScrollNumber = scrollNumber;
	currentSetNumber = setNumber;
	resetSetNumber = false;
}

