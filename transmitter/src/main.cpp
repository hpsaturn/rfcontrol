/**
 * RF Remote control via 315 Mhz module. 
 * This code transmits messages using the VirtualWire library,
 * connect the Transmitter data pin to Arduino pin 12
 */

#include <Arduino.h>
#include <VirtualWire.h>

int pbIn0Up = 8;
int pbIn1Dn = 9;
int pbIn2Lf = 10;
int pbIn3Rg = 11;

void send(char *message) {
  vw_send((uint8_t *)message, strlen(message));
  vw_wait_tx(); // Wait until the whole message is gone
}

void setup() {
  // Initialize the IO and ISR
  vw_setup(2000); // Bits per sec
  pinMode(pbIn0Up,INPUT_PULLUP);
  pinMode(pbIn1Dn,INPUT_PULLUP);
  pinMode(pbIn2Lf,INPUT_PULLUP);
  pinMode(pbIn3Rg,INPUT_PULLUP);
}

void loop() {
  if(digitalRead(pbIn0Up)==HIGH) send("u");
  if(digitalRead(pbIn1Dn)==HIGH) send("d");
  delay(10);
}

