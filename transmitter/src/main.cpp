/*
 * Blink
 * Turns on an LED on for one second,
 * then off for one second, repeatedly.
 */

#include <Arduino.h>
#include <VirtualWire.h>

void send(char *message)
{
  vw_send((uint8_t *)message, strlen(message));
  vw_wait_tx(); // Wait until the whole message is gone
}

void setup()
{
  // Initialize the IO and ISR
  vw_setup(2000); // Bits per sec
}

void loop()
{
  send("p");
}

