/**
 * RF Remote control via 315 Mhz module. 
 * This code transmits messages using the VirtualWire library,
 * connect the Transmitter data pin to Arduino pin 12
 */

#include <Arduino.h>
#include <VirtualWire.h>

int pbIn0Up = 8;
int pbIn1Dn = 2;
int pbIn2Lf = 3;
int pbIn3Rg = 4;

void send(char *message) {
  vw_send((uint8_t *)message, strlen(message));
  vw_wait_tx(); // Wait until the whole message is gone
}

void setup() {
  // Initialize the IO and ISR
  vw_setup(3000); // Bits per sec
  pinMode(pbIn0Up,INPUT_PULLUP);
  pinMode(pbIn1Dn,INPUT_PULLUP);
  pinMode(pbIn2Lf,INPUT_PULLUP);
  pinMode(pbIn3Rg,INPUT_PULLUP);
}

void loop() {
  if(!digitalRead(pbIn0Up)) send("u");
  if(!digitalRead(pbIn1Dn)) send("d");
  if(!digitalRead(pbIn2Lf)) send("l");
  if(!digitalRead(pbIn3Rg)) send("r");
}

