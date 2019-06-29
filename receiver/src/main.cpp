/**
 * RF Remote control via 315 Mhz module. 
 * This code transmits messages using the VirtualWire library,
 * Connect the Receiver data pin to Arduino pin 11
 */

#include <Arduino.h>
#include <VirtualWire.h>

#define musicPin   4
#define motorPinL  5
#define motorPinR  6
#define led_pin   13

int motor_val = 0;
int motor_dir = 0;
bool toogleDir = false;
unsigned long lastReceiveTime = 0;
unsigned long currentTime = 0;

// Max size of this struct is 32 bytes ?
struct Data_Package {
  byte j1PotX;
  byte j1PotY;
  byte j1Button;
  byte j2PotX;
  byte j2PotY;
  byte j2Button;
  byte tSwitch1;
  byte tSwitch2;
  byte buttonUp;
  byte buttonDn;
  byte buttonLf;
  byte buttonRg;
};

byte message[VW_MAX_MESSAGE_LEN];        // a buffer to store the incoming messages
byte messageLength = VW_MAX_MESSAGE_LEN; // the size of the message

Data_Package data;

void resetData() {
  // Reset the values when there is no radio connection - Set initial default values
  data.j1PotX = 127;
  data.j1PotY = 127;
  data.j2PotX = 127;
  data.j2PotY = 127;
  data.j1Button = 1;
  data.j2Button = 1;
  data.tSwitch1 = 1;
  data.tSwitch2 = 1;
  data.buttonUp = 1;
  data.buttonDn = 1;
  data.buttonLf = 1;
  data.buttonRg = 1;
}

void receiveMessage() {
  if (vw_get_message(message, &messageLength)) { // Non-blocking
    // if (vw_get_message((uint8_t)&data, (uint8_t)sizeof(Data_Package))) { // Non-blocking
    digitalWrite(led_pin, HIGH); // Flash a light to show received good message
    Serial.print("msg: ");
    for (int i = 0; i < messageLength; i++)
    {
      Serial.print(message[i]);
    }
    digitalWrite(led_pin, LOW);
    Serial.println("");
  }
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

void oldMotorControl(byte msg){
  switch (msg)
  {
  case (char)'u':
    Serial.print("motorUp: ");
    if (motor_val >= 0 && motor_val < 255)
    {
      motor_val = motor_val + 1;
      motorSpeed(motor_val, motor_dir);
    }
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
  // Check whether we keep receving data, or we have a connection between the two modules
  currentTime = millis();
  if ( currentTime - lastReceiveTime > 2000 ) { // that means we have lost connection.
   resetData(); // If connection is lost, reset the data.
  }

  if (vw_get_message((uint8_t *)&data, sizeof(Data_Package))){
    Serial.print(" btnUp:");
    Serial.print(data.buttonUp);
    Serial.print(" btnDn:");
    Serial.print(data.buttonDn);
    Serial.print(" btnLf:");
    Serial.print(data.buttonLf);
    Serial.print(" btnRg:");
    Serial.println(data.buttonRg);
    lastReceiveTime = millis(); // At this moment we have received the data
  }

}
