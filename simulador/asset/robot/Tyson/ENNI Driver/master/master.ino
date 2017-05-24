/*
ENNI Software - Multiple serial communication

Receives from the three software serial ports,
sends to the hardware serial port host.

In order to listen on a software port, you call port.listen().
When using two software serial ports, you have to switch ports
by listen()ing on each one in turn. Pick a logical time to switch
ports, like the end of an expected transmission, or when the
buffer is empty. This example switches ports when there is nothing
more to read from a port

The circuit:
Three devices which communicate serially are needed.
* First serial device's TX attached to digital pin 10(RX), RX to pin 11(TX)
* Second serial device's TX attached to digital pin 12(RX), RX to pin 13(TX)
* Third serial device's TX attached to digital pin 50(RX), RX to pin 51(TX)

Note:
Not all pins on the Mega and Mega 2560 support change interrupts,
so only the following can be used for RX:
L, 52,  53, 62, 63, 64, 65, 66, 67, 68, 69

Not all pins on the Leonardo support change interrupts,
so only the following can be used for RX:
8, 9, 10, 11, 14 (MISO), 15 (SCK), 16 (MOSI).

Master start listening serial

By default the slaves start listening on the serial port 
Every slave that receive a serial function has a callback when the process is completed.

**TO DO: 
-Complete Doc
-Modify Code to parallelize functions and receive asynchronous callbacks
-Create Exceptions/Solution in case communication is lost

Command params: SLAVE_SELECTOR//MASTER COMMAND PARAMS...
e.g. SLAVE_X MOVE_TO_POS 1800

created 19 May 2017
modified 19 May 2017
by Alfredo Pizaña
based on Tom Igoe "Two Port Receive"

*/

#include <SoftwareSerial.h>
//Master Board Arduino Mega 
// Note: On the Mega, use other pins instead, since 8 and 9 don't work on the Mega

// software serial #1: 
//Slave Serial for X Axis RX = digital pin 10, TX = digital pin 11
SoftwareSerial SlaveX(10, 11);

// software serial #2: 
//Slave Serial for Y Axis RX = digital pin 12, TX = digital pin 13
SoftwareSerial SlaveY(12, 13);

// software serial #3: 
//Slave Serial for Z Axis RX = digital pin 50, TX = digital pin 51
SoftwareSerial SlaveZ(50, 51);

//Flags for comumnication
//Reading = false *** Writing = true
boolean SerialX = false;
boolean SerialY = false;
boolean SerialZ = false;



String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete
String callbackString = "";

#define MAX_COMMAND_PARAMS 6 //IMPORTANT: Modify this constant if a longer command is needed

String command[MAX_COMMAND_PARAMS];



void setup() {
	// Open serial communications and wait for port to open:
	Serial.begin(9600);

	// reserve 200 bytes for the inputString:
	inputString.reserve(200);
	while (!Serial) {
		; // wait for serial port to connect. Needed for native USB port only
	}


	// Start each software serial port
	SlaveX.begin(9600);
	SlaveY.begin(9600);
	SlaveZ.begin(9600);
	Serial.println("Setup Master Completed");
}


void loop() {

	// print the string when a newline arrives:
	if (stringComplete) {

		int iCount = StringSplit(inputString,' ',command, MAX_COMMAND_PARAMS);

		if (iCount > 0)
			evaluateCommand(command, iCount);
		else
			Serial.println("The command was not recognized");

		// clear the string:
		inputString = "";
		stringComplete = false;
	}
	

}

/*
SerialEvent occurs whenever a new data comes in the
hardware serial RX.  This routine is run between each
time loop() runs, so using delay inside loop can delay
response.  Multiple bytes of data may be available.
*/
void serialEvent() {
	while (Serial.available() && !stringComplete) {
		// get the new byte:
		char inChar = (char)Serial.read();
		// add it to the inputString:
		inputString += inChar;
		// if the incoming character is a newline, set a flag
		// so the main loop can do something about it:
		if (inChar == '\n') {
			stringComplete = true;
		}

	}
}



//Function to split a String
int StringSplit(String sInput, char cDelim, String sParams[], int iMaxParams)
{
	int iParamCount = 0;
	int iPosDelim, iPosStart = 0;
	do {
		// Searching the delimiter using indexOf()
		iPosDelim = sInput.indexOf(cDelim, iPosStart);
		if (iPosDelim > (iPosStart + 1)) {
			// Adding a new parameter using substring() 
			sParams[iParamCount] = sInput.substring(iPosStart, iPosDelim);
			iParamCount++;
			// Checking the number of parameters
			if (iParamCount >= iMaxParams) {
				return (iParamCount);
			}
			iPosStart = iPosDelim + 1;
		}
	} while (iPosDelim >= 0);
	if (iParamCount < iMaxParams) {
		// Adding the last parameter as the end of the line
		sParams[iParamCount] = sInput.substring(iPosStart);
		iParamCount++;
	}
	return (iParamCount);
}


void evaluateCommand(String cCommand[], int iMaxParams) {

	String slaveCommand = "";
	for (int i = 1; i < iMaxParams; i++)
	{
		slaveCommand.concat(cCommand[i]);
		if((i+1)< iMaxParams){
			slaveCommand.concat(' ');
		}	
	}

	if (cCommand[0].equals("MASTER")) {

	}
	else if (cCommand[0].equals("SLAVE_X")) {

		SlaveX.write(slaveCommand.concat('\n'));
		SlaveX.listen();
		String callback = "";
		if (SlaveX.available()) {
			callback = SlaveX.readString();
		}

	}
	else if (cCommand[0].equals("SLAVE_Y")) {
		SlaveY.write(slaveCommand.concat('\n'));
		SlaveY.listen();
		String callback = "";
		if (SlaveY.available()) {
			callback = SlaveY.readString();
		}
	}
	else if (cCommand[0].equals("SLAVE_Z")) {
		SlaveZ.write(slaveCommand.concat('\n'));
		SlaveZ.listen();
		String callback = "";
		if (SlaveZ.available()) {
			callback = SlaveZ.readString();
		}

	}
	else {
		Serial.println("The command was not recognized!!");
	}
}