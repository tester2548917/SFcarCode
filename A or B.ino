#include <SparkFun_TB6612.h>

// Motor Driver Pins
#define AIN1 9
#define BIN1 7
#define AIN2 10
#define BIN2 6
#define PWMA 11
#define PWMB 5
#define STBY 8

// IR Sensor Pins
#define R A0
#define R2 A1
#define R3 A2
#define L3 A3
#define L2 A4
#define L A5

#define SENSOR_FAR 700
#define SENSOR_CLOSE 400
#define MIN_SPEED 80

// Threshold for obstacle detection (adjust as needed)
#define THRESHOLD 300  // Higher value means closer obstacle

// Motor objects
Motor motorLeft = Motor(AIN1, AIN2, PWMA, -1, STBY);
Motor motorRight = Motor(BIN1, BIN2, PWMB, -1, STBY);

void setup() {
  // Initialize serial communication for debugging
  Serial.begin(115200);
  
  // Set IR sensor pins as inputs
  pinMode(R, INPUT);
  pinMode(R2, INPUT);
  pinMode(R3, INPUT);
  pinMode(L3, INPUT);
  pinMode(L2, INPUT);
  pinMode(L, INPUT);
}

void loop() {
  int rightDistance = analogRead(R);
  int r2 = analogRead(R2);
  int r3 = analogRead(R3);
  int l3 = analogRead(L3);
  int l2 = analogRead(L2);
  int leftDistance = analogRead(L);

  // Print sensor readings
/*   Serial.print(leftDistance);
  Serial.print(",");
  Serial.print(middleDistance);
  Serial.print(",");
  Serial.println(rightDistance); */

  // Calculate motor speeds based on sensor readings
  int leftMotorSpeed = map(leftDistance, SENSOR_CLOSE, SENSOR_FAR, -255, 255);
  int l2MotorSpeed = map(l2, SENSOR_CLOSE, SENSOR_FAR, -255, 255);
  int l3MotorSpeed = map(l3, SENSOR_CLOSE, SENSOR_FAR, 255, -255);
  int r3MotorSpeed = map(r3, SENSOR_CLOSE, SENSOR_FAR, 255, -255);
  int r2MotorSpeed = map(r2, SENSOR_CLOSE, SENSOR_FAR, -255, 255);
  int rightMotorSpeed = map(rightDistance, SENSOR_CLOSE, SENSOR_FAR, -255, 255);
/*   int forwardSpeed = map(middleDistance, SENSOR_CLOSE, SENSOR_FAR, 255, -255); */

  // Adjust speeds for obstacle avoidance

  int combinedLeftSpeed = l3MotorSpeed*2-leftMotorSpeed-l2MotorSpeed;
  int combinedRightSpeed = r3MotorSpeed*2-rightMotorSpeed-r2MotorSpeed;


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
  motorLeft.drive(combinedLeftSpeed);
  motorRight.drive(combinedRightSpeed);

  delay(100);  // Short delay to avoid too frequent updates
}