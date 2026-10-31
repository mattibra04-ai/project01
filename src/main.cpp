#include <Arduino.h>

// Dual H-bridge wiring (for example, an L298N):
// Left front + rear motors share channel A; right front + rear share channel B.
// Remove the ENA/ENB jumpers if your driver has them, so PWM can control speed.
const uint8_t LEFT_ENABLE = 5;  // PWM
const uint8_t LEFT_IN1 = 7;
const uint8_t LEFT_IN2 = 8;
const uint8_t RIGHT_ENABLE = 6; // PWM
const uint8_t RIGHT_IN1 = 9;
const uint8_t RIGHT_IN2 = 10;

const uint8_t MOTOR_SPEED = 180; // 0..255

void setSide(uint8_t enablePin, uint8_t in1, uint8_t in2, int direction) {
  if (direction > 0) {
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
  } else if (direction < 0) {
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
  } else {
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
  }
  analogWrite(enablePin, direction == 0 ? 0 : MOTOR_SPEED);
}

void drive(int leftDirection, int rightDirection) {
  setSide(LEFT_ENABLE, LEFT_IN1, LEFT_IN2, leftDirection);
  setSide(RIGHT_ENABLE, RIGHT_IN1, RIGHT_IN2, rightDirection);
}

void stopMotors() { drive(0, 0); }
void forward()    { drive(1, 1); }
void reverse()    { drive(-1, -1); }
void turnLeft()   { drive(-1, 1); }
void turnRight()  { drive(1, -1); }

void setup() {
  pinMode(LEFT_ENABLE, OUTPUT);
  pinMode(LEFT_IN1, OUTPUT);
  pinMode(LEFT_IN2, OUTPUT);
  pinMode(RIGHT_ENABLE, OUTPUT);
  pinMode(RIGHT_IN1, OUTPUT);
  pinMode(RIGHT_IN2, OUTPUT);
  stopMotors();

  Serial.begin(9600);
  Serial.println(F("Four-wheel drive ready. Commands: W forward, S reverse, A left, D right, X stop."));
}

void loop() {
  if (Serial.available() == 0) return;

  const char command = (char)Serial.read();
  switch (command) {
    case 'w': case 'W': forward();   Serial.println(F("Forward")); break;
    case 's': case 'S': reverse();   Serial.println(F("Reverse")); break;
    case 'a': case 'A': turnLeft();  Serial.println(F("Left")); break;
    case 'd': case 'D': turnRight(); Serial.println(F("Right")); break;
    case 'x': case 'X': stopMotors();Serial.println(F("Stop")); break;
    case '\r': case '\n': break;
    default: Serial.println(F("Use W, A, S, D, or X.")); break;
  }
}
