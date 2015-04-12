#include <I2C.h>
#include <LaserSupport.h>
#include <NewPing.h>
#include <MotorMovement.h>

//DEFINES/CONSTANTS
#define SonarTriggerPin   14
#define SonarEchoPin      15
#define SonarMaxDistance  255

#define PinLeftForwards   10
#define PinRightForwards  11
#define PinLeftReverse    12
#define PinRightReverse   13

//OBJECTS
AsymmetricMotorPair engines(PinLeftForwards, PinRightForwards, PinLeftReverse, PinRightReverse);
NewPing sonarObject(SonarTriggerPin, SonarEchoPin, SonarMaxDistance);
//I2CLaser laserObject(0);

//GLOBAL VARIABLES
command currentCommand = STOP;

void setup()
{
	//set data rate for the Serial port
	Serial.begin(9600);
}

void loop()
{
	//query the serial interface to check if there are bytes to read
	if (Serial.available())
	{
		currentCommand = convertToCommand(Serial.read());
	}

	switch (currentCommand)
	{
	case DRIVEFORWARDS:
		if (sonarObject.convert_cm(sonarObject.ping_median()) > 20)         
		{
			engines.driveForward(20, 20); 
			break;
		}
		else { currentCommand = STOP; }
		
	case DRIVEBACKWARDS:
		 engines.driveBackward(20, 20); 
		 break;
	case TURNLEFT:
		engines.turnLeft(20);
		break;
	case TURNRIGHT:
		engines.turnRight(20);
		break;
	case ROTATELEFT:
		engines.turnLeftOnPoint(20, 20);
		break;
	case ROTATERIGHT:
		engines.turnRightOnPoint(20, 20);
		break;
	case STOP:
		engines.stop();
		break;
	default:
		//as backup 
		engines.stop();
		break;
	}
}

//The purpose was to make the code more user friendy to my engineering friends who cant into programming
//Because the character codes might have not been as informative but the enumeration is.
command convertToCommand(char incomingChar)
{
	switch (incomingChar)
	{
	case '0':
		return STOP;
		break;
	case '2':
		return DRIVEBACKWARDS;
		break;
	case '4':
		return TURNLEFT;
		break;
	case '6':
		return TURNRIGHT;
		break;
	case '8':
		return DRIVEFORWARDS;
		break;
	case 'E':
		return ROTATELEFT;
		break;
	case 'F':
		return ROTATERIGHT;
		break;
	default:
		return STOP;
		break;
	}
}