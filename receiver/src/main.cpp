/**
 * RF Remote control receiver via 315 Mhz module. 
 * This code receive messages, Data_Package structure, 
 * using the VirtualWire library.
 */

#include <Arduino.h>
#include <VirtualWire.h>

#define led_pin   13
#define musicPin   4
#define motorPinL  5
#define motorPinR  6
#define increment  5

uint8_t motor_val = 0;
uint8_t motor_dir = 0;
bool toogleDir = false;
bool toogleMusic = false;
unsigned long lastReceiveTime = 0;
unsigned long currentTime = 0;

// Max size of this struct is 32 bytes ?
struct Data_Package {
  uint8_t j1PotX;
  uint8_t j1PotY;
  uint8_t j1Button;
  uint8_t tSwitch1;
  uint8_t tSwitch2;
  uint8_t buttonUp;
  uint8_t buttonDn;
  uint8_t buttonLf;
  uint8_t buttonRg;
};

Data_Package data;

void resetData() {
  // Reset the values when there is no radio connection - Set initial default values
  data.j1PotX = 127;
  data.j1PotY = 127;
  data.j1Button = 1;
  data.tSwitch1 = 1;
  data.tSwitch2 = 1;
  data.buttonUp = 1;
  data.buttonDn = 1;
  data.buttonLf = 1;
  data.buttonRg = 1;
}

void printData() {
  Serial.print(" Jx:"); Serial.print(data.j1PotX);
  Serial.print(" Jy:"); Serial.print(data.j1PotY);
  Serial.print(" Up:"); Serial.print(data.buttonUp);
  Serial.print(" Dn:"); Serial.print(data.buttonDn);
  Serial.print(" Lf:"); Serial.print(data.buttonLf);
  Serial.print(" Rg:"); Serial.print(data.buttonRg);
  Serial.print(" MV:"); Serial.print(motor_val);
  Serial.print(" MD:"); Serial.println(motor_dir);
}

void musicCheck() {
  if (toogleMusic) digitalWrite(musicPin, HIGH);
  else digitalWrite(musicPin,LOW);
}

void motorSpeed (int value, int dir){
  if (dir == 0) {
    analogWrite(motorPinL, 0);
    analogWrite(motorPinR, value);
  }
  else if (dir == 1) {
    analogWrite(motorPinR, 0);
    analogWrite(motorPinL, value);
  }
}

void speedUp () {
  if(motor_val <= (255-increment))
    motor_val = motor_val + increment;
}

void speedDn () {
  if(motor_val >= increment)
    motor_val = motor_val - increment;
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
  resetData();
}

void loop() {
  // Check whether we keep receving data, or we have a connection between the two modules
  // currentTime = millis();
  // if ( currentTime - lastReceiveTime > 2000 ) { // that means we have lost connection.
  //  resetData(); // If connection is lost, reset the data.
  // }
 
  uint8_t data_len = sizeof(Data_Package);

  if (vw_get_message((uint8_t*)&data, &data_len)){
    // lastReceiveTime = millis(); // At this moment we have received the data
    if (data.buttonUp == 0 ) speedUp();
    if (data.buttonDn == 0 ) speedDn();
    if (data.buttonRg == 0 ) motor_dir = 0;
    if (data.buttonLf == 0 ) motor_dir = 1;
    if (data.buttonLf == 0 && data.buttonRg == 0) toogleMusic = !toogleMusic;

    int xAxis = data.j1PotX;
    int yAxis = data.j1PotY;
    int motorSpeed = 0;

    // Y-axis used for increasing or decreasing motor value control
    if (yAxis > 140) {
      // Convert the increasing Y-axis readings from 140 to 255 into 0 to 255 value for the PWM signal
      motorSpeed = map(yAxis, 140, 255, 0, 255);
      if(motorSpeed > motor_val) motor_val = motorSpeed;
    }
    else if (yAxis < 110) {
      // Convert the declining Y-axis readings from 110 to 0 into 0 to 255 value for the PWM signal 
      motorSpeed = map(yAxis, 110, 0, 255, 0);
      if(motorSpeed < motor_val) motor_val = motorSpeed;
    }
    // X-axis used for backward and forward control
    if (xAxis < 110) {
      motorSpeed = map(xAxis, 110, 0, 255, 0);
      if(motorSpeed < motor_val) motor_val = motorSpeed;
      motor_dir = 1;
    }
    if (xAxis > 140) {
      motorSpeed = map(xAxis, 140, 255, 0, 255);
      if(motorSpeed > motor_val) motor_val = motorSpeed;
      motor_dir = 0;
    }
    // Serial.print("dlen:"); Serial.print(data_len);
    // printData();
  }

  musicCheck();  // on/off music chip
  motorSpeed(motor_val,motor_dir);
}
