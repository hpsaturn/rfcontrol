/**
 * RF Remote control via 315 Mhz module. 
 * This code transmits messages using the VirtualWire library,
 * connect the Transmitter data pin to Arduino pin 12
 */

#include <Arduino.h>
#include <VirtualWire.h>

// Define the digital inputs
#define jB1 1   // Joystick button 1
#define jB2 0   // Joystick button 2
#define t1 7    // Toggle switch 1
#define t2 4    // Toggle switch 1
#define btnUp 8 // cross button Up
#define btnDn 2 // cross button Down
#define btnLf 3 // cross button Left
#define btnRg 4 // cross button Right

// Max size of this struct is 32 bytes ?
struct Data_Package {
  byte j1PotX;
  byte j1PotY;
  byte j1Button;
  byte tSwitch1;
  byte tSwitch2;
  byte buttonUp;
  byte buttonDn;
  byte buttonLf;
  byte buttonRg;
};

Data_Package data; //Create a variable with the above structure

void send(char *message) {
  vw_send((uint8_t *)message, strlen(message));
  vw_wait_tx(); // Wait until the whole message is gone
}

void setup() {
  Serial.begin(9600);
  // Initialize the IO and ISR
  vw_setup(2000); // Bits per sec
  pinMode(jB1, INPUT_PULLUP);
  pinMode(jB2, INPUT_PULLUP);
  pinMode(t1, INPUT_PULLUP);
  pinMode(t2, INPUT_PULLUP);
  pinMode(btnUp,INPUT_PULLUP);
  pinMode(btnDn,INPUT_PULLUP);
  pinMode(btnLf,INPUT_PULLUP);
  pinMode(btnRg,INPUT_PULLUP);

  // Set initial default values
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

void loop() {
  // Read all analog inputs and map them to one Byte value
  data.j1PotX = map(analogRead(A1), 0, 1023, 0, 255); // Convert the analog read value from 0 to 1023 into a BYTE value from 0 to 255
  data.j1PotY = map(analogRead(A0), 0, 1023, 0, 255);
  // Read all digital inputs
  data.j1Button = digitalRead(jB1);
  data.tSwitch1 = digitalRead(t1);
  data.tSwitch2 = digitalRead(t2);
  data.buttonUp = digitalRead(btnUp);
  data.buttonDn = digitalRead(btnDn);
  data.buttonLf = digitalRead(btnLf);
  data.buttonRg = digitalRead(btnRg);

  vw_send((uint8_t*)&data, sizeof(Data_Package));
  vw_wait_tx(); // Wait until the whole message is gone
}
