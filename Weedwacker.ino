#include <AFMotor.h>
#include <ezButton.h>

//ARDUINO OBSTACLE AVOIDING CAR//
// Before uploading the code you have to install the necessary library//
//AFMotor Library https://learn.adafruit.com/adafruit-motor-shield/library-install //
//NewPing Library https://github.com/livetronic/Arduino-NewPing// 
//Servo Library https://github.com/arduino-libraries/Servo.git //
// To Install the libraries go to sketch >> Include Library >> Add .ZIP File >> Select the Downloaded ZIP files From the Above links //

#include <NewPing.h>
#include <Servo.h> 

#define TRIG_PIN A0 
#define ECHO_PIN A1 
#define TRIG_PIN_A A3 
#define ECHO_PIN_A A2 
#define TRIG_PIN_B A5 
#define ECHO_PIN_B A4                 // CHANGE
#define TRIG_PIN_C 13 
#define ECHO_PIN_C 12 
#define MAX_DISTANCE 200 
#define MAX_SPEED 190 // sets speed of DC  motors
#define MAX_SPEED_OFFSET 20

NewPing sonar(TRIG_PIN, ECHO_PIN, MAX_DISTANCE); 


Servo myservo;   

boolean goesForward=false;
int distance = 100;
int minDelta = 5;         // cm
int speedSet = 0;
int FBmotor1pin1 = 2;
int FBmotor1pin2 = 3;
int LRmotor2pin1 = 4;
int LRmotor2pin2 = 5;
int LS_LED = 7;
int enFB = 9;            // no speed control needed for LR
long duration;
long distance_A_zero;
long distance_B_zero;
long distance_C_zero;
long distance_A_new;
long distance_B_new;
long distance_C_new;
long deltaA;
long deltaB;
long deltaC;
long delta;
int Cutmotorpin1 = 8;         
int Cutmotorpin2 = 10;

void setup() {
  
  pinMode(TRIG_PIN_A, OUTPUT); // Sets the trigPin as an OUTPUT
  pinMode(ECHO_PIN_A, INPUT); // Sets the echoPin as an INPUT
  pinMode(TRIG_PIN_B, OUTPUT); // Sets the trigPin as an OUTPUT
  pinMode(ECHO_PIN_B, INPUT); // Sets the echoPin as an INPUT
  pinMode(TRIG_PIN_C, OUTPUT); // Sets the trigPin as an OUTPUT
  pinMode(ECHO_PIN_C, INPUT); // Sets the echoPin as an INPUT
  digitalWrite(TRIG_PIN_A, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN_A, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN_A, LOW);
  duration = pulseIn(ECHO_PIN_A, HIGH);
  distance_A_zero = duration * 0.034 / 2;
  digitalWrite(TRIG_PIN_B, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN_B, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN_B, LOW);
  duration = pulseIn(ECHO_PIN_B, HIGH);
  distance_B_zero = duration * 0.034 / 2;
  digitalWrite(TRIG_PIN_C, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN_C, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN_C, LOW);
  duration = pulseIn(ECHO_PIN_C, HIGH);
  distance_C_zero = duration * 0.034 / 2;
  
  myservo.attach(11);           //Servo pwm attached to pin 11
  pinMode(FBmotor1pin1, OUTPUT);
  pinMode(FBmotor1pin2, OUTPUT);
  pinMode(LRmotor2pin1, OUTPUT);
  pinMode(LRmotor2pin2, OUTPUT);
  pinMode(Cutmotorpin1, OUTPUT);
  pinMode(Cutmotorpin2, OUTPUT);
  pinMode(LS_LED, OUTPUT);
  pinMode(enFB, OUTPUT); 
  myservo.write(115); 
  delay(2000);
  distance = readPing();
  delay(100);
  distance = readPing();
  delay(100);
  distance = readPing();
  delay(100);
  distance = readPing();
  delay(100);
  //Serial.begin(9600); // // Serial Communication is starting with 9600 of baudrate speed

}

void loop() {
 int distanceR = 0;
 int distanceL =  0;
 delay(40);
 
 if(distance<=15)
 {
  moveStop();
  delay(100);
  moveBackward();
  delay(300);
  moveStop();
  delay(200);
  distanceR = lookRight();
  delay(200);
  distanceL = lookLeft();
  delay(200);

  if(distanceR>=distanceL)
  {
    turnRight();
    moveStop();
  }else
  {
    turnLeft();
    moveStop();
  }
 }else
 {
  moveForward();
 }

 if(delta > minDelta){
   digitalWrite(LS_LED,HIGH);
   moveStop();
   Cut();
   delay(1000);
   digitalWrite(LS_LED,LOW);
   moveForward();}
 distance = readPing();
 int delta = ScanGround();
}

int lookRight()
{
    myservo.write(50);                 // right
    delay(500);
    int distance = readPing();  
    delay(100);         
    myservo.write(115);                 //neutral
    return distance;
}

int lookLeft()
{
    myservo.write(170);                 //left
    delay(500);
    int distance = readPing();
    delay(100);
    myservo.write(115); 
    return distance;
    delay(100);
}

int readPing() { 
  delay(70);
  int cm = sonar.ping_cm();
  if(cm==0)
  {
    cm = 250;
  }
  return cm;
}

int ScanGround() { 
  digitalWrite(TRIG_PIN_A, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN_A, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN_A, LOW);
  duration = pulseIn(ECHO_PIN_A, HIGH);
  distance_A_new = duration * 0.034 / 2;
  deltaA = distance_A_zero - distance_A_new;    // positive indicates object closer
  
  digitalWrite(TRIG_PIN_B, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN_B, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN_B, LOW);
  duration = pulseIn(ECHO_PIN_B, HIGH);
  distance_B_new = duration * 0.034 / 2;
  deltaB = distance_B_zero - distance_B_new;    // positive indicates object closer
  
  digitalWrite(TRIG_PIN_C, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN_C, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN_C, LOW);
  duration = pulseIn(ECHO_PIN_C, HIGH);
  distance_C_new = duration * 0.034 / 2;
  deltaC = distance_C_zero - distance_C_new;    // positive indicates object closer

  delta = max(deltaA,deltaB);
  delta = max(delta, deltaC);
}














void moveStop() {
   digitalWrite(FBmotor1pin1, LOW);
   digitalWrite(FBmotor1pin2, LOW); 
  } 
  
void moveForward() {

// if(!goesForward)
//  {
    //goesForward=true;
   digitalWrite(FBmotor1pin1, HIGH);
   digitalWrite(FBmotor1pin2, LOW);   
//   for (speedSet = 0; speedSet < MAX_SPEED; speedSet +=2) // slowly bring the speed up to avoid loading down the batteries too quickly
//   {
//    analogWrite(enFB,speedSet);
//    delay(5);
//   }
//  }
}

void moveBackward() {
    goesForward=false;
   digitalWrite(FBmotor1pin1, LOW);
   digitalWrite(FBmotor1pin2, HIGH);  
//  for (speedSet = 0; speedSet < MAX_SPEED; speedSet +=2) // slowly bring the speed up to avoid loading down the batteries too quickly
//  {
//    analogWrite(enFB,speedSet);
//    delay(5);
//  }
}  

void turnRight() {
   digitalWrite(LRmotor2pin1, LOW);
   digitalWrite(LRmotor2pin2, HIGH);
   digitalWrite(FBmotor1pin1, LOW);
   digitalWrite(FBmotor1pin2, HIGH);     // PULSE!!!
   delay(1000);
   moveStop();
   delay(500);
   digitalWrite(LRmotor2pin1, LOW);
   digitalWrite(LRmotor2pin2, HIGH);
   digitalWrite(FBmotor1pin1, LOW);
   digitalWrite(FBmotor1pin2, HIGH);     // PULSE!!!
   delay(1000);
   moveStop();
   delay(500);
   digitalWrite(LRmotor2pin1, LOW);
   digitalWrite(LRmotor2pin2, HIGH);
   digitalWrite(FBmotor1pin1, LOW);
   digitalWrite(FBmotor1pin2, HIGH);     // PULSE!!!
   delay(1000);
   moveStop();
   delay(500);
   digitalWrite(LRmotor2pin1, LOW);
   digitalWrite(LRmotor2pin2, HIGH);
   digitalWrite(FBmotor1pin1, LOW);
   digitalWrite(FBmotor1pin2, HIGH);     // PULSE!!!
   delay(1000);
   moveStop();
   delay(500);
   
   digitalWrite(LRmotor2pin1, LOW);
   digitalWrite(LRmotor2pin2, LOW);
} 
 
void turnLeft() {
   digitalWrite(LRmotor2pin1, HIGH);
   digitalWrite(LRmotor2pin2, LOW);
   digitalWrite(FBmotor1pin1, LOW);
   digitalWrite(FBmotor1pin2, HIGH); 
   delay(1000);
   moveStop();
   delay(500);
   digitalWrite(LRmotor2pin1, HIGH);
   digitalWrite(LRmotor2pin2, LOW);
   digitalWrite(FBmotor1pin1, LOW);
   digitalWrite(FBmotor1pin2, HIGH); 
   delay(1000);
   moveStop();
   delay(500);
      digitalWrite(LRmotor2pin1, HIGH);
   digitalWrite(LRmotor2pin2, LOW);
   digitalWrite(FBmotor1pin1, LOW);
   digitalWrite(FBmotor1pin2, HIGH); 
   delay(1000);
   moveStop();
   delay(500);
   digitalWrite(LRmotor2pin1, HIGH);
   digitalWrite(LRmotor2pin2, LOW);
   digitalWrite(FBmotor1pin1, LOW);
   digitalWrite(FBmotor1pin2, HIGH); 
   delay(1000);
   moveStop();
   delay(500);
   
  digitalWrite(LRmotor2pin1, LOW);
  digitalWrite(LRmotor2pin2, LOW);
}  



void Cut(){
   digitalWrite(Cutmotorpin1, LOW);
   digitalWrite(Cutmotorpin2, HIGH); 
   delay(1000);
   digitalWrite(Cutmotorpin1, LOW);
   digitalWrite(Cutmotorpin2, LOW); 
}
