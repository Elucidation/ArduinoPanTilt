// Pan/Tilt servo control over serial link
#include <Servo.h>
#include <ArduinoJson.h>

// Servo pins
#define SERVO_PAN_PIN 10
#define SERVO_TILT_PIN 9

// Pan servo center min/max range (0-180 degrees with 90 degrees centred)
#define SERVO_PAN_ZERO 90
#define SERVO_PAN_MIN 10
#define SERVO_PAN_MAX 160

// Tilt servo center min/max range (0-180 degrees with 90 degrees centred)
#define SERVO_TILT_ZERO 90
#define SERVO_TILT_MIN 10
#define SERVO_TILT_MAX 160

// Precision for Up, Down, Left and Right operations
#define PREC 5

DynamicJsonDocument doc(1024);

// Servo objects
Servo servo_pan;
Servo servo_tilt;

// Servo positions (0-180 degrees with 90 degrees centred)
int pan_pos = 0;
int tilt_pos = 0;
int pan_deg = 0;
int tilt_deg = 0;

// Serial read variables
String inputString = "";         // A string to hold incoming data
boolean stringComplete = false;  // Whether the string is complete


// Sets pan and tilt degrees, with 0,0 facing forward
// Give values in range -45, 45 degrees for both
void setPanTilt(int pan_deg, int tilt_deg)
{
  setPan(pan_deg);
  delay(100);
  setTilt(tilt_deg);

  // Wait for servos to get to position
  delay(100);
}

void setPan(int s_pan_deg)
{
  int current = pan_deg + SERVO_PAN_ZERO;
  // Remember degrees
  pan_deg = s_pan_deg;
  // Set 90 degrees center for servo
  pan_pos = s_pan_deg + SERVO_PAN_ZERO;

  // Constrain angles to limits
  pan_pos = constrain(pan_pos, SERVO_PAN_MIN, SERVO_PAN_MAX);
  if(pan_pos > current){
    while(pan_pos >= current){
      current++;
      servo_pan.write(current);
      delay(100);
    }
  }
  else{
    while(pan_pos <= current){
      current--;
      servo_pan.write(current);
      delay(100);
    }
  }
  // Write angles to servo
  // servo_pan.write(pan_pos);
}

void setTilt(int s_tilt_deg)
{
  int current = tilt_deg + SERVO_PAN_ZERO;
  // Remember degrees
  tilt_deg = s_tilt_deg;
  // Set 90 degrees center for servo
  tilt_pos = s_tilt_deg + SERVO_TILT_ZERO;

  // Constrain angles to limits
  tilt_pos = constrain(tilt_pos, SERVO_TILT_MIN, SERVO_TILT_MAX);
  if(tilt_pos > current){
    while(tilt_pos >= current){
      current++;
      servo_tilt.write(current);
      delay(100);
    }
  }
  else{
    while(tilt_pos <= current){
      current--;
      servo_tilt.write(current);
      delay(100);
    }
  }
  // Write angles to servo
  // servo_tilt.write(tilt_pos);
}

void printServoPositions() // TODO - change into json data
{
  // Relative positions
  doc["PanDeg"] = pan_deg;
  doc["TiltDeg"] = tilt_deg;
  // Absolute positions
  doc["PanPos"] = pan_pos;
  doc["TiltPos"] = tilt_pos;
  serializeJson(doc, Serial);
  Serial.println();
}

/*
  SerialEvent occurs whenever a new data comes in the
 hardware serial RX.  This routine is run between each
 time loop() runs, so using delay inside loop can delay
 response.  Multiple bytes of data may be available.

 Commands:
  s PAN_ANGLE TILT_ANGLE      --- Set Pan Tilt positions in degrees
  p PAN_ANGLE                 --- Set Pan  position in degrees
  t TILT_ANGLE                --- Set Tilt position in degrees
  z                           --- Zero out to default center positions
  i                           --- Info on current pan/tilt servo angles/positions
  u                           --- Up by PREC amount
  d                           --- Down by PREC amount
  l                           --- Left by PREC amount
  r                           --- Right by PREC amount

 */
void serialEvent()
{
  while (Serial.available())
  {
    // Get state character from buffer
    char c = (char)Serial.read();

    switch (c)
    {
    // s pan-position-degree tilt-position-degrees
    case 's':
    {
      // Where degrees are zero-centered to camera facing forward
      int pan = Serial.parseInt();
      int tilt = Serial.parseInt();
      setPanTilt(pan, tilt);
      printServoPositions();
      break;
    }

    case 'p':
    {
      // Set pan degree
      int pan = Serial.parseInt();
      setPan(pan);
      printServoPositions();
      break;
    }

    case 't':
    {
      // Set tilt degree
      int tilt = Serial.parseInt();
      setTilt(tilt);
      printServoPositions();
      break;
    }
        
    case 'u':
    {
      // Up
      int tilt = tilt_deg + PREC;
      setTilt(tilt);
      printServoPositions();
      break;
    }

    case 'd':
    {
      // Down
      int tilt = tilt_deg - PREC;
      setTilt(tilt);
      printServoPositions();
      break;
    }

    case 'l':
    {
      // Left
      int pan = pan_deg + PREC;
      setPan(pan);
      printServoPositions();
      break;
    }

    case 'r':
    {
      // Right
      int pan = pan_deg - PREC;
      setPan(pan);
      printServoPositions();
      break;
    }

    case 'z':
    {
      // zero out and recenter
      setPanTilt(0, 0);
      break;
    }

    case 'i':
    {
      // Print current servo positions
      printServoPositions();
      break;
    }
    }
  }
}

void setup()
{
  // Initialize serial
  Serial.begin(9600);
  // Reserve 200 bytes for the inputString:
  inputString.reserve(200);

  // Attach pins
  servo_pan.attach(SERVO_PAN_PIN);
  servo_tilt.attach(SERVO_TILT_PIN);

  Serial.println("Setting pan/tilt to zero center...");
  setPanTilt(0, 0);
  printServoPositions();
}

void loop()
{
}
