/**
 * RF Remote control via 315 Mhz module. 
 * This code transmits messages using the VirtualWire library,
 * Connect the Receiver data pin to Arduino pin 11
 */

#include <Arduino.h>
#include <VirtualWire.h>

byte message[VW_MAX_MESSAGE_LEN];        // a buffer to store the incoming messages
byte messageLength = VW_MAX_MESSAGE_LEN; // the size of the message

int motor_val = 0;
int motor_dir = 0;
int motorPinL = 5;
int motorPinR = 6;
bool toogleDir = false;

byte receiveMessage() {
  if (vw_get_message(message, &messageLength)) { // Non-blocking
    if (messageLength == 1) return message[0];
  }
  return 0;
}

void motorSpeed (int value, int dir){
  if (dir == 0 ) {
    analogWrite(motorPinL, 0);
    analogWrite(motorPinR, value);
  }
  else if (dir == 1 ) {
    analogWrite(motorPinR, 0);
    analogWrite(motorPinL, value);
  }
}

void motorTest() {
  if(toogleDir) motorSpeed (motor_val,0);
  else motorSpeed (motor_val,1);

  if(motor_val++ > 255) {
    motor_val=0;
    toogleDir = !toogleDir;
  }
}

void setup()
{
  Serial.begin(9600);
  Serial.println("Device is ready");
  // Initialize the IO and ISR
  vw_setup(2000); // Bits per sec
  vw_rx_start();  // Start the receiver
  pinMode(motorPinL,OUTPUT);
  pinMode(motorPinR,OUTPUT);
}

void loop() {

  byte msg = receiveMessage();

  switch (msg)
  {
  case (char)'u':
    Serial.print("motorUp: ");
    if (motor_val >= 0 && motor_val < 255)
      motorSpeed(motor_val++, motor_dir);
    Serial.println(motor_val);
    break;

  case (char)'d':
    Serial.print("motorDn: ");
    if (motor_val > 0 && motor_val < 256)
      motorSpeed(motor_val--, motor_dir);
    Serial.println(motor_val);
    break;

  case (char)'l':
    Serial.println("motorDir: left");
    motorSpeed(motor_val, 0);
    break;

  case (char)'r':
    Serial.println("motorDir: right");
    motorSpeed(motor_val, 1);
    break;
  }

  delay(10);
}
