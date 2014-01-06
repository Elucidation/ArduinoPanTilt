// Pan/Tilt servo control over serial link
#include <Servo.h>

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

// Servo objects
Servo servo_pan;
Servo servo_tilt;

// Servo positions (0-180 degrees with 90 degrees centred)
int pan_pos = 0;
int tilt_pos = 0;

// Serial read variables
String inputString = "";         // A string to hold incoming data
boolean stringComplete = false;  // Whether the string is complete


// Sets pan and tilt degrees, with 0,0 facing forward
// Give values in range -45, 45 degrees for both
void setPanTilt(int pan_deg, int tilt_deg)
{
  setPan(pan_deg);
  setTilt(tilt_deg);

  // Wait for servos to get to position
  delay(100);
}

void setPan(int pan_deg)
{
  // Set 90 degrees center for servo
  pan_pos = pan_deg + SERVO_PAN_ZERO;

  // Constrain angles to limits
  pan_pos = constrain(pan_pos, SERVO_PAN_MIN, SERVO_PAN_MAX);

  // Write angles to servo
  servo_pan.write(pan_pos);
}

void setTilt(int tilt_deg)
{
  // Set 90 degrees center for servo
  tilt_pos = tilt_deg + SERVO_TILT_ZERO;

  // Constrain angles to limits
  tilt_pos = constrain(tilt_pos, SERVO_TILT_MIN, SERVO_TILT_MAX);

  // Write angles to servo
  servo_tilt.write(tilt_pos);
}

void printServoPositions()
{
  int pan_deg = pan_pos - SERVO_PAN_ZERO;
  int tilt_deg = tilt_pos - SERVO_TILT_ZERO;

  Serial.print("Servo Angles in Degrees (pan/tilt): ");
  Serial.print(pan_deg);
  Serial.print(" ");
  Serial.println(tilt_deg);

  Serial.print("Servo Actual Positions (pan/tilt): ");
  Serial.print(pan_pos);
  Serial.print(" ");
  Serial.println(tilt_pos);
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