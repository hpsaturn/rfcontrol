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
#define increment  5

int motor_val = 0;
int motor_dir = 0;
bool toogleDir = false;
bool toogleMusic = false;
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

void printData() {
  Serial.print(" Up:");
  Serial.print(data.buttonUp);
  Serial.print(" Dn:");
  Serial.print(data.buttonDn);
  Serial.print(" Lf:");
  Serial.print(data.buttonLf);
  Serial.print(" Rg:");
  Serial.println(data.buttonRg);
}

void musicCheck() {
  if (toogleMusic) digitalWrite(musicPin, HIGH);
  else digitalWrite(musicPin,LOW);
}

void motorSpeed (int value, int dir){
  if (dir == 0 && value < 255 && value > 0) {
    analogWrite(motorPinL, 0);
    analogWrite(motorPinR, value);
  }
  else if (dir == 1 && value < 255 && value > 0) {
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

void speedUp () {
  motor_val = motor_val + increment;
  if(motor_val>255)motor_val = 255;
  Serial.println(motor_val);
}

void speedDn () {
  motor_val = motor_val - increment;
  if(motor_val<0)motor_val = 0;
  Serial.println(motor_val);
}

void setup() {
  Serial.begin(9600);
  Serial.println("Device is ready");
  // Initialize the IO and ISR
  vw_setup(2000); // Bits per sec
  vw_rx_start();  // Start the receiver
  pinMode(motorPinL,OUTPUT);
  pinMode(motorPinR,OUTPUT);
  pinMode(musicPin, OUTPUT);
}

void loop() {
  // Check whether we keep receving data, or we have a connection between the two modules
  currentTime = millis();
  if ( currentTime - lastReceiveTime > 2000 ) { // that means we have lost connection.
   resetData(); // If connection is lost, reset the data.
  }

  if (vw_get_message((uint8_t *)&data, sizeof(Data_Package))){
    //printData();
    lastReceiveTime = millis(); // At this moment we have received the data

    if (data.buttonUp == 0 ) speedUp();
    if (data.buttonDn == 0 ) speedDn();
    if (data.buttonRg == 0 ) motor_dir = 0;
    if (data.buttonLf == 0 ) motor_dir = 1;
    if (data.buttonLf == 0 && data.buttonRg == 0)toogleMusic = !toogleMusic;
  }

  musicCheck();
  motorSpeed(motor_val,motor_dir);
}
