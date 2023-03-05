#include <Servo.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

Servo xAxis;  // create servo object to control a servo
Servo yAxis;
Servo VisorServo;

float angle = 0;
float CameraToMirrorX = 15;   // Presets In Inches
float CameraToMirrorY = 0;
float CameraToMirrorZ = 16;

float CurrentX = 0.0;
float CurrentY = 0.0;
float CurrentZ = 0.0;

float TangentRadiansXY = 0;
float TangentDegreesXY = 0;

float TangentRadiansYZ = 0;
float TangentDegreesYZ = 0;

float CameraToPersonY = 0.0;
float CameraToPersonX = 0.0;
float CameraToPersonZ = 0.0;

int OutputAngleXY = 0;
int OutputAngleYZ = 0;

const int ButtonPin = 7;
const int SecondButtonPin = 5;
int ButtonState = 0;


const int TopPhotoR = A3; // Photoresistor at Arduino analog pin A3 
const int MidPhotoR = A4; // Photoresistor at Arduino analong pin A4 
const int LowPhotoR = A2; // Photo resistor at Arduino analong pin A2 
 
int TopPhotoR_Value = 0; 
int MidPhotoR_Value = 0; 
int LowPhotoR_Value = 0; 
int counter = 0; 

int HomePosition = 140;         // Home Position of Sun Visor
int ExtendedPosition = 0;   // Extended Position of Visor

const int trigPin = 9;
const int echoPin = 10;

long duration;
int distance;

void setup() {
  Serial.begin(9600);
   
  lcd.init();
  lcd.clear();         
  lcd.backlight();      // Make sure backlight is on
  
  xAxis.attach(13);  // attaches the servo on pin 13 to the servo object
  yAxis.attach(12);
  VisorServo.attach(8);

  pinMode(TopPhotoR, INPUT);  // Set TopPhotoR - A3 
  pinMode(MidPhotoR, INPUT);  // Set MidPhotoR - A4 
  pinMode(LowPhotoR, INPUT);  // Set LowPhotoR - A5 

  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
}

void loop() {
  
  ButtonState = digitalRead(ButtonPin) or digitalRead(SecondButtonPin);
  
  while (ButtonState == LOW){
    ButtonState = digitalRead(ButtonPin) or digitalRead(SecondButtonPin);
    delay(200); 
  }
  
  Serial.println("Pressed");

  bool recFlag = false; 

  while(!recFlag){
    String str = ""; 
    if (Serial.available() > 0) {  // check if data available
      String str = Serial.readString();  // read string from serial
      delay(250);
  
     // parse the string into three floats using strtok and atof
      char* token = strtok(const_cast<char*>(str.c_str()), ",");
      float x = atof(token);
      token = strtok(NULL, ",");
      float y = atof(token);
      token = strtok(NULL, ",");
      float z = atof(token);
  
      Serial.println("x = " + String(x) + ", y = " + String(y) + ", z = " + String(z));
      recFlag = true; 

      CameraToPersonX = ((x * 24) - 12);  // Values relative to camera location
      CameraToPersonY = -1 * ((y * 24) - 12);
      CameraToPersonZ = 0;
    }
  }

  EverythingElseFunction();
  PhotoResistorStuff();
  SonarStuff();
  
  delay(1000);
}

void EverythingElseFunction() {
  
  CurrentX = CameraToMirrorX + CameraToPersonX; // Add relative value to preset
  CurrentY = CameraToMirrorY + CameraToPersonY; 
  CurrentZ = CameraToMirrorZ + CameraToPersonZ;

  lcd.setCursor(0,0);   
  lcd.print("X: " + String(CurrentX));
  
  lcd.setCursor(8,0);   
  lcd.print("Y: " + String(CurrentY));

//  lcd.setCursor(0,1);   
//  lcd.print("Z: " + String(CurrentZ));

  
  TangentRadiansXY = atan(CurrentY / CurrentZ);
  TangentRadiansYZ = atan(CurrentX / CurrentZ);


  TangentDegreesXY = TangentRadiansXY * 180.0 / 3.14159; 
  TangentDegreesYZ = TangentRadiansYZ * 180.0 / 3.14159;



  OutputAngleXY = 100.0 + (TangentDegreesXY / 2.3);
  OutputAngleYZ = 100.0 + (TangentDegreesYZ / 2.0);

  
  lcd.setCursor(0,1);   
  lcd.print("Xd: " + String(OutputAngleXY));

  lcd.setCursor(8,1);   
  lcd.print("Yd: " + String(OutputAngleYZ));
  


  if (OutputAngleXY > 140 ) {
    OutputAngleXY = 140;
  }

  if (OutputAngleYZ > 130 ) {
    OutputAngleYZ = 130;
  }

  if (OutputAngleXY < 70 ) {
    OutputAngleXY = 70;
  }

  if (OutputAngleYZ < 80 ) {
    OutputAngleYZ = 80;
  }


  yAxis.write(OutputAngleXY);
  xAxis.write(OutputAngleYZ);

}

void PhotoResistorStuff() {
  TopPhotoR_Value = analogRead(TopPhotoR); 
  MidPhotoR_Value = analogRead(MidPhotoR); 
  LowPhotoR_Value = analogRead(LowPhotoR); 
  
  //You can change value "25" 
  
  if ((TopPhotoR_Value >  200)){ 
  //Keep visor up 
    
    VisorServo.write(HomePosition);
   
   
  } 
  
  else  { 
  VisorServo.write(ExtendedPosition);

  } 
}


void SonarStuff() {
    // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distance = duration * 0.034 / 2;

}
