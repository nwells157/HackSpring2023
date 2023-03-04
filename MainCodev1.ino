/*********
  Rui Santos
  Complete project details at https://randomnerdtutorials.com  
  Written by BARRAGAN and modified by Scott Fitzgerald
*********/

#include <Servo.h>

Servo xAxis;  // create servo object to control a servo
Servo yAxis;
Servo zAxis;

int angle = 0;
int CameraToMirrorX = 1; // Preset Constant
int CameraToMirrorY = 1;


int CameraX = 1;  // Values relative to camera location
int CameraY = 1;
int CameraZ = 1;

int CurrentX = 1;
int CurrentY = 1;
int CurrentZ = 1;

int TangentRadiansXY = 0;
int TangentDegreesXY = 0;

int CameraToPersonY = 1;
int CameraToPersonX = 1;

int OutputAngleXY = 0;

void setup() {
  Serial.begin(9600);
  
  xAxis.attach(13);  // attaches the servo on pin 13 to the servo object
  yAxis.attach(12);
  zAxis.attach(8);
}

void loop() {

  // read the input on analog pin 0:
  int sensorValue = analogRead(A0);
  // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
  float DegreesXY = sensorValue / 1023.0 * 90.0;
  // print out the value you read:
  Serial.println("SensorValue : " + String(sensorValue));
  Serial.println("Degrees : " + String(DegreesXY));
  
  CurrentX = CameraToMirrorX + CameraX; // Add relative value to preset
  CurrentY = CameraToPersonY + CameraY;
  TangentRadiansXY = tan(CurrentY / CurrentX);

  TangentDegreesXY = TangentRadiansXY * 180 / 3.14; 
  OutputAngleXY = TangentDegreesXY - (90-TangentDegreesXY) / 2;
  Serial.println("XY Degrees = " + String(TangentDegreesXY));
  Serial.println("OutputAngle = " + String(OutputAngleXY));
  xAxis.write(DegreesXY); // Replace with OutputAngleXY
  // delay(00);
                           

}
