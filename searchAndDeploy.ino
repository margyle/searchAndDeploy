#include <Servo.h>
Servo myservo1;
Servo myservo2;
int pos = 0;

//--------------------------
//set up serial listeners
const byte numChars = 32;
char receivedChars[numChars];
// temporary array for use when parsing
char tempChars[numChars];        
//--------------------------


//--------------------------
// variables to hold the parsed data
char queryType[numChars] = {0};
int servo1Pos = 0;
int servo2Pos = 0;
float stepperSpeed = 0.0;
boolean newData = false;
//--------------------------

//--------------------------
//queryTypes
char moveQuery[] ="M";
//--------------------------


void setup() {
    //confirm system is online
    Serial.begin(19200);

    delay(100);
    //attach and zero servos
    myservo1.attach(11);
    myservo1.write(0);
    myservo2.attach(12);
    myservo2.write(0);
    myservo2.write(90);
    delay(1000);
    Serial.print("200");
    Serial.println("Search and Deploy: Listening");
    }



void loop() {

unsigned long currentMillis_1 = millis();
unsigned long currentMillis_stepper = millis();
  
if (newData == true) {
      strcpy(tempChars, receivedChars);
      parseData();
      newData = false;
   }
recvWithStartEndMarkers();
}

//-------------------------- START recvWithStartEndMarkers
void recvWithStartEndMarkers() {
    static boolean recvInProgress = false;
    static byte ndx = 0;
    char startMarker = '<';
    char endMarker = '>';
    char rc;

    while (Serial.available() > 0 && newData == false) {
        rc = Serial.read();
        if (recvInProgress == true) {
            if (rc != endMarker) {
                receivedChars[ndx] = rc;
                ndx++;
                if (ndx >= numChars) {
                    ndx = numChars - 1;
                }
            }
            else {
                receivedChars[ndx] = '\0'; // terminate the string
                recvInProgress = false;
                ndx = 0;
                newData = true;
            }
        }
        else if (rc == startMarker) {
            recvInProgress = true;
        }
    }
}

//-------------------------- END recvWithStartEndMarkers


void parseData() {      
  char * strtokIndx; 

//get query type
  strtokIndx = strtok(tempChars,","); // get the first part - the string
  strcpy(queryType, strtokIndx);
    


    strtokIndx = strtok(NULL, ","); // get the first part - the string
    servo1Pos = atoi(strtokIndx); // copy it to servo2
 
    strtokIndx = strtok(NULL, ","); // this continues where the previous call left off
    servo2Pos = atoi(strtokIndx);     // convert this part to an integer
    
    moveLaser();
    systemResponse();
    
}
//-------------------------- END parseData

void moveLaser(){
  myservo1.write(servo1Pos);
  delay(10);
  myservo2.write(servo2Pos);
  delay(10);
}
void systemResponse(){
  Serial.print("Command Recieved: ");
   Serial.println(servo1Pos);
   Serial.println(servo2Pos);
}
