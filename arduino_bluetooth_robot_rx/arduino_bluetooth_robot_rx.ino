/* Arduino Multi-mode Bluetooth Robot

 The circuit:
 * D11   >>>  Rx
 * D10   >>>  Tx
 * 5V    >>>  VCC (some HC-05 models are powered on 3.3V, verify before connecting)
 * GND   >>>  GND
 Note:
 * you will need arduino 1.0.1 or higher to run this sketch
 
 Author: Andrew R. from HobbyTransform (http://hobbytransform.com/), written in 2016
*/

#include <SoftwareSerial.h>       // import the serial library

#define motor1Forward 3
#define motor1Reverse 5
#define motor2Forward 6
#define motor2Reverse 11

#define LIGHT_FOLLOW      '1'
#define LIGHT_AVOID       '2'
#define LINE_FOLLOW       '3'
#define ULTRASONIC_FOLLOW '4'
#define ULTRASONIC_AVOID  '5'
#define MOVE_FORWARD      'w'
#define MOVE_BACK         's'
#define MOVE_LEFT         'a'
#define MOVE_RIGHT        'd'

SoftwareSerial BTserial(7, 8);    // RX, TX

// Constants
int light[4] = {0,1,2,3};         //wire as ldrLeft A0, ldrRight A1, lineLeft A2, lineRight A3
int lightTol[2] = {10,15};        //light tol, line tol
int driveTime = 3;

// Variables
int leftMotorSpeed = 0;
int rightMotorSpeed = 0;

void setup(){
  Serial.begin(9600);
  Serial.print("setup");

  // setup peripherals: bluetooth serial
  BTserial.begin(9600);
  
  // Set Pins
  pinMode(motor1Forward, OUTPUT);
  pinMode(motor1Reverse, OUTPUT);
  pinMode(motor2Forward, OUTPUT);
  pinMode(motor2Reverse, OUTPUT);
  
  // Initialize Pins
  resetMotors();
  
  delay(1000);  //allow me to anticipate startup
}

void loop(){
  
  char BluetoothByte;              // the data given from Computer

  if(BTserial.available()){
    
    BluetoothByte = BTserial.read();

    switch(BluetoothByte){
      
      case LIGHT_FOLLOW:
      
        light_mode(0,'F');
        break;
        
      case LIGHT_AVOID:
      
        light_mode(0,'A');
        break;
        
      case LINE_FOLLOW:
        
        light_mode(1,'A');  // Line Follow (avoid white)
        break;
        
      case ULTRASONIC_FOLLOW:
              
        break;
        
      case ULTRASONIC_AVOID:
              
        break;
        
      case MOVE_FORWARD:
      
        // RC mode: forward
        forward(HIGH);
        break;
        
      case MOVE_BACK:
      
        // RC mode: reverse
        reverse(HIGH);
        break;
        
      case MOVE_LEFT:
      
        // RC mode: steer left
        steerLeft(HIGH);
        break;
        
      case MOVE_RIGHT:
      
        // RC mode: steer right
        steerRight(HIGH);
        break;
        
    }
  }
}

// Helper Functions
void light_mode(int s, char m){
  //LDR: 0 with 'F' or 'A';   Line Follow: 1 with 'A'
  int left = analogRead(light[2*s]);
  int right = analogRead(light[2*s+1]);
  
  if(abs(left - right) > lightTol[s]){
    if(left > right){
      if(m == 'F'){
        steerRight(HIGH);
      } else if(m == 'A'){
        steerLeft(HIGH);
      }
    }
    else{
      if(m == 'F'){
        steerLeft(HIGH);
      } else if(m == 'A'){
        steerRight(HIGH);
      }
    }
  }
  else{
    forward(HIGH);
  }
}

void resetMotors(){
  digitalWrite(motor1Forward, LOW);
  digitalWrite(motor1Reverse, LOW);
  digitalWrite(motor2Forward, LOW);
  digitalWrite(motor2Reverse, LOW);
}

void drive(int leftSpeed,int rightSpeed){
  if(leftSpeed >= 0){
    analogWrite(motor1Reverse, LOW);
    analogWrite(motor1Forward, leftSpeed);
  } else if(leftSpeed < 0){
    analogWrite(motor1Forward, LOW);
    analogWrite(motor1Reverse, abs(leftSpeed));
  }
  if(rightSpeed >= 0){
    analogWrite(motor2Reverse, LOW);
    analogWrite(motor2Forward, rightSpeed);
  } else if(rightSpeed < 0){
    analogWrite(motor2Forward, LOW);
    analogWrite(motor2Reverse, abs(rightSpeed));
  }
  delay(driveTime);
}

void forward(int motorSpeed){
  analogWrite(motor1Reverse, LOW);
  analogWrite(motor1Forward, motorSpeed);
  analogWrite(motor2Reverse, LOW);
  analogWrite(motor2Forward, motorSpeed);
  delay(driveTime);
}

void reverse(int motorSpeed){
  analogWrite(motor1Forward, LOW);
  analogWrite(motor1Reverse, motorSpeed);
  analogWrite(motor2Forward, LOW);
  analogWrite(motor2Reverse, motorSpeed);
  delay(driveTime);
}

void steerLeft(int motorSpeed){
  analogWrite(motor1Forward, LOW);
  analogWrite(motor1Reverse, motorSpeed);
  analogWrite(motor2Reverse, LOW);
  analogWrite(motor2Forward, motorSpeed);
  delay(driveTime);
}

void steerRight(int motorSpeed){
  analogWrite(motor1Reverse, LOW);
  analogWrite(motor1Forward, motorSpeed);
  analogWrite(motor2Forward, LOW);
  analogWrite(motor2Reverse, motorSpeed);
  delay(driveTime);
}
