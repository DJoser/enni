//------------------------------------------------------------------- Define Module
#define MODULE_NAME "base"

void sendData(char* data){
  Serial.print(MODULE_NAME);
  Serial.println(data);
}
//------------------------------------------------------------------- Define Sensor
#define sensorR 8
#define sensorL 7

//------------------------------------------------------------------- Define Motor
#define motorR 6
#define motorL 5
#define motorENA 9
int motorSpeed = 180;
#define LEFT -1
#define STOP 0
#define RIGHT 1
int motorDirection = STOP;

void motorRotateLeft(){
  digitalWrite(motorR,HIGH);
  digitalWrite(motorL,LOW);
  motorDirection = STOP;
}

void motorRotateRight(){
  digitalWrite(motorR,LOW);
  digitalWrite(motorL,HIGH );
  motorDirection = RIGHT;
}

void motorStop(){
  digitalWrite(motorR,LOW);
  digitalWrite(motorL,LOW);
  motorDirection = LEFT;
}

void motorSetSpeed(int value){
  if(value >= 0 || value <= 255){
    motorSpeed = value;
  }else{
    Serial.print("Invalid motor speed value");
  }
}

//------------------------------------------------------------------- Define Encoder
#define encoderA 4
#define encoderB 3
#define encoderZ 2

int encoderCounter = 0;
int encoderA_State;
int encoderA_LastState;

int encoderLoopCounter = 0;
int encoderZ_State;
int encoderZ_LastState;

bool encoderSendCounter = false;
bool encoderSendLoopCounter = false;

void encoderReset(){
  encoderCounter = 0;
  encoderLoopCounter = 0;
}

void encoderLogCounter(){
  if(encoderSendCounter){
    Serial.print("position : ");
    Serial.println(encoderCounter);
  }
}

void encoderLogLoopCounter(){
  if(encoderSendLoopCounter){
    Serial.print("loop : ");
    Serial.println(encoderLoopCounter);
  }
}

//------------------------------------------------------------------- Define Serial Comunication
#define MAX_COMMAND_PARAMS 3 //IMPORTANT: Modify this constant if a longer command is needed
String command[MAX_COMMAND_PARAMS];
String inputString = "";         // a string to hold incoming data
/*
SerialEvent occurs whenever a new data comes in the
hardware serial RX.  This routine is run between each
time loop() runs, so using delay inside loop can delay
response.  Multiple bytes of data may be available.
*/
void serialEvent() {
  
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    
    if (inChar == ';') {
      Serial.println(inputString);
      int iCount = StringSplit(inputString, ' ', command, MAX_COMMAND_PARAMS);
      evaluateCommand(command);
      inputString = "";
    }else{
      inputString += inChar; // add it to the inputString:
    }
  }
}

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

void evaluateCommand(String cCommand[]) {

  //--------------------------------------------------------------------------- MOTOR
  if (cCommand[0].equals("MOTOR")){
    //------------------------------------------------------------------------------------------------ ROTATE
    if (cCommand[1].equals("ROTATE")){
      if(cCommand[2].equals("LEFT")){
        motorRotateLeft();
      }else if(cCommand[2].equals("STOP")){
        motorStop();
      }else if(cCommand[2].equals("RIGHT")){
        motorRotateRight();
      }else{
        commandError();
      }
    }
    //------------------------------------------------------------------------------------------------ SPEED
    else if (cCommand[1].equals("SPEED")){
      motorSetSpeed(cCommand[2].toInt());
    }
    //------------------------------------------------------------------------------------------------ MOTOR ERROR
    else{
      commandError();
    }
  }
  //--------------------------------------------------------------------------- ENCODER
  else if (cCommand[0].equals("ENCODER")) {
    //------------------------------------------------------------------------------------------- RESET
    if (cCommand[1].equals("RESET")){
      encoderReset();
    }
    //------------------------------------------------------------------------------------------- LOG COUNTER
    else if(cCommand[1].equals("LOGC")){
      if(cCommand[2].equals("ON")){
        encoderSendCounter = true;
      }else if(cCommand[2].equals("OFF")){
        encoderSendCounter = false;
      }else{
        commandError();
      }
    }
    //------------------------------------------------------------------------------------------- LOG LOOPER
    else if(cCommand[1].equals("LOGL")){
      if(cCommand[2].equals("ON")){
        encoderSendLoopCounter = true;
      }else if(cCommand[2].equals("OFF")){
        encoderSendLoopCounter = false;
      }else{
        commandError();
      }
    }
    //------------------------------------------------------------------------------------------- ENCODER ERROR
    else{
      commandError();
    }
  }
  //--------------------------------------------------------------------------- ERROR
  else {
    commandError();
  }
}

void commandError(){
  Serial.print("SLAVE_ERROR COMMAND_NOT_FOUND \n");
}
//------------------------------------------------------------------- Main Program

void setup(){
  // Setup Motor
  pinMode(motorR,OUTPUT);
  pinMode(motorL,OUTPUT);
  pinMode(motorSpeed,OUTPUT);
  motorStop();

  // Setup Encoder
  pinMode(encoderA,INPUT);
  pinMode(encoderB,INPUT);
  pinMode(encoderZ,INPUT);
  encoderA_LastState = digitalRead(encoderA);

  // Setup Communication
  inputString.reserve(200); // reserve 200 bytes for the inputString:
  Serial.begin(9600);
  while (!Serial) { ; } // wait for serial port to connect. Needed for native USB port only
}

void loop(){
  //-------------------------------------------------------- Motor loop
  analogWrite(motorENA,motorSpeed);
  //-------------------------------------------------------- Encoder AB Loop
  encoderA_State = digitalRead(encoderA);
  if(encoderA_State != encoderA_LastState){
    if(digitalRead(encoderB) != encoderA_State){
      encoderCounter = encoderCounter + 1;
    }else{
      encoderCounter = encoderCounter - 1;
    }
    encoderLogCounter();
  }
  encoderA_LastState = encoderA_State;

  //-------------------------------------------------------- Encoder Z Loop
  encoderZ_State = digitalRead(encoderZ);
  if(encoderZ_State != encoderZ_LastState){
    if(motorDirection == LEFT){
      encoderLoopCounter = encoderLoopCounter - 1;
    }else if(motorDirection == RIGHT){
      encoderLoopCounter = encoderLoopCounter + 1;
    }else{
      // Motor Stoped
    }
    encoderLogLoopCounter();
  }
  encoderZ_LastState = encoderZ_State;
}

