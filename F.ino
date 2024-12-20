#include <SparkFun_TB6612.h>
#include <Servo.h>                                          //servo controller library
#include <OneButton.h>   
// Motor Driver Pins

#define KP 0.4
#define KD 0.8
//

#define AIN1 9
#define BIN1 7
#define AIN2 10
#define BIN2 6
#define PWMA 11
#define PWMB 5
#define STBY 8
#define KP 0.4  
// IR Sensor Pins

//OBJ DETECTION
#define RIGHT_SENSOR A4
#define RIGHTC_SENSOR_PIN A4
//#define LMIDDLE_SENSOR A2
//#define RMIDDLE_SENSOR A3
#define LEFTC_SENSOR_PIN A1
#define LEFT_SENSOR A1

//LANE DETECTION
#define LEFT_SENSOR_PIN   A5                                //Left line sensor pin
#define RIGHTB_SENSOR_PIN  A3
#define LEFTB_SENSOR_PIN  A2
#define RIGHT_SENSOR_PIN  A0

#define SENSOR_FAR 700
#define SENSOR_CLOSE 400
#define MIN_SPEED 80

int leftSensorValue, rightSensorValue, leftSensorValueb, rightSensorValueb, leftSensorValuec, rightSensorValuec;                      //variables for storing the value of the line sensors


int combinedmotorspeed, leftMotorSpeed, rightMotorSpeed;    //creating variables like this initializes all their values to 0.

// Define variables for PID control
float lastError, derivative, controlSignal;                 //these initialize to 0/
int error;
int TOP_SPEED = 0; 

// Threshold for obstacle detection (adjust as needed)
#define THRESHOLD 300  // Higher value means closer obstacle

// Motor objects

Motor motorLeft = Motor(AIN1, AIN2, PWMA, 1, STBY);
Motor motorRight = Motor(BIN1, BIN2, PWMB, 1, STBY);

void setup() {
  // Initialize serial communication for debugging
  Serial.begin(115200);
  
  // Set IR sensor pins as inputs
  pinMode(RIGHT_SENSOR, INPUT);

  //pinMode(LMIDDLE_SENSOR, INPUT);
  //pinMode(RMIDDLE_SENSOR, INPUT);
  pinMode(LEFT_SENSOR, INPUT);

}

void loop() {
  int rightDistance = analogRead(RIGHT_SENSOR);
  //int lmiddleDistance = analogRead(LMIDDLE_SENSOR);
  //int rmiddleDistance = analogRead(RMIDDLE_SENSOR);
  int leftDistance = analogRead(LEFT_SENSOR);

  // Print sensor readings

  leftSensorValue = analogRead(LEFT_SENSOR_PIN);                //Read line sensor values
  leftSensorValueb = analogRead(LEFTB_SENSOR_PIN);
  leftSensorValuec = analogRead(LEFTC_SENSOR_PIN);
  rightSensorValue = analogRead(RIGHT_SENSOR_PIN);
  rightSensorValueb = analogRead(RIGHTB_SENSOR_PIN);
  rightSensorValuec = analogRead(RIGHTC_SENSOR_PIN);
  error = (leftSensorValueb + leftSensorValue+ leftSensorValuec) - (rightSensorValueb + rightSensorValue + rightSensorValuec);                   //Calculate the error (difference between the two sensor values)
  derivative = error - lastError;                               //Calculate the derivative term, which is how fast the error is changing
  controlSignal = (KP * error) + (KD * derivative);             //Calculate the control signal
  lastError = error;

  // Calculate motor speeds based on sensor readings
  int leftMotorSpeed = map(leftDistance, SENSOR_CLOSE, SENSOR_FAR, -255, 255);
  int rightMotorSpeed = map(rightDistance, SENSOR_CLOSE, SENSOR_FAR, -255, 255);
//  int lforwardSpeed = map(lmiddleDistance, SENSOR_CLOSE, SENSOR_FAR, 255, -255);
//  int rforwardSpeed = map(rmiddleDistance, SENSOR_CLOSE, SENSOR_FAR, 255, -255);
  // Adjust speeds for obstacle avoidance
  int combinedLeftSpeed = leftMotorSpeed;
  int combinedRightSpeed = rightMotorSpeed;

  // Constrain speeds to valid range
  combinedLeftSpeed = constrain(combinedLeftSpeed, -255, 255);
  combinedRightSpeed = constrain(combinedRightSpeed, -255, 255);

  if (abs(combinedRightSpeed) < MIN_SPEED){
    if(combinedRightSpeed>0){
      combinedRightSpeed = MIN_SPEED;
    }
    else{
      combinedRightSpeed = -MIN_SPEED-20;
    }
  }
  if (abs(combinedLeftSpeed) < MIN_SPEED){
    if(combinedLeftSpeed>0){
      combinedLeftSpeed = MIN_SPEED;
    }
    else{
      combinedLeftSpeed = -MIN_SPEED-20;
    }
  }
  // Set motor speeds

  leftMotorSpeed = 255 - controlSignal;                   //If the error is positive, this slows down the motor on the left
  rightMotorSpeed = 255 + controlSignal; 
  
  leftMotorSpeed = constrain(leftMotorSpeed, 0, 255);     //This makes the speed be somewhere between 0 and TOP_SPEED
  rightMotorSpeed = constrain(rightMotorSpeed, 0, 255);

/*   if (combinedLeftSpeed < leftMotorSpeed) {
    motorLeft.drive(combinedLeftSpeed);
    motorRight.drive(combinedRightSpeed);}
  else if (combinedRightSpeed < rightMotorSpeed) {
    motorLeft.drive(combinedLeftSpeed);
    motorRight.drive(combinedRightSpeed);
  }
  else {
  motorLeft.drive(leftMotorSpeed);
  motorRight.drive(rightMotorSpeed);
  } */
  
  motorLeft.drive(leftMotorSpeed);
  motorRight.drive(rightMotorSpeed);

/* if (leftDistance < 255) {
  motorRight.drive(-combinedRightSpeed);
  motorLeft.drive(-combinedLeftSpeed);
}
else {
  motorLeft.drive(leftMotorSpeed);
  motorRight.drive(rightMotorSpeed);
}

if (lmiddleDistance < 255) {
  motorRight.drive(-combinedRightSpeed);
  motorLeft.drive(-combinedLeftSpeed);
}
else {
  motorLeft.drive(leftMotorSpeed);
  motorRight.drive(rightMotorSpeed);
}

if (rmiddleDistance < 255) {
  motorRight.drive(-combinedRightSpeed);
  motorLeft.drive(-combinedLeftSpeed);
}
else {
  motorLeft.drive(leftMotorSpeed);
  motorRight.drive(rightMotorSpeed);
}

if (rightDistance < 255) {
  motorRight.drive(-combinedRightSpeed);
  motorLeft.drive(-combinedLeftSpeed);
}
else {
  motorLeft.drive(leftMotorSpeed);
  motorRight.drive(rightMotorSpeed);
} */



 //delay(100);  // Short delay to avoid too frequent updates
}