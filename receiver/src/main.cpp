#include <Arduino.h>
#include <VirtualWire.h>

byte message[VW_MAX_MESSAGE_LEN];        // a buffer to store the incoming messages
byte messageLength = VW_MAX_MESSAGE_LEN; // the size of the message

int motor_val = 0;
int motorPinL = 5;
int motorPinR = 6;

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
void loop()
{
  if (vw_get_message(message, &messageLength)) // Non-blocking
  {
    Serial.print("Received: ");
    for (int i = 0; i < messageLength; i++)
    {
      Serial.write(message[i]);
    }
    Serial.println();
  }

  analogWrite(motorPinL,0);
  analogWrite(motorPinR,motor_val++);
  if(motor_val>255)motor_val=0;
  delay(100);

}
