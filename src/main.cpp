#include <Arduino.h>
#include <Wire.h>
#include "ros.h"

byte J12_buffer[4] = { 0 };
byte J345_buffer[6] = { 0 };

void send_joint_positions(float Joints[5])
{
  int16_t joint_buffer[5] = { 0 };


  for (int i=0; i<5; i++)
  {
    joint_buffer[i] = Joints[i] * 1000;
  }

  for(int i=0; i<2; i++)
  {
    J12_buffer[2*i] = joint_buffer[i] & 0xFF;
    J12_buffer[2*i+1] = joint_buffer[i] >> 8;
  }

  for(int i=0; i<3; i++)
  {
    J345_buffer[2*i] = joint_buffer[i+2] & 0xFF;
    J345_buffer[2*i+1] = joint_buffer[i+2] >> 8;
  }

  Wire.beginTransmission(0x02);
  Wire.write(*J12_buffer);
  Wire.endTransmission();

  Wire.beginTransmission(0x03);
  Wire.write(*J345_buffer);
  Wire.endTransmission();
}

void setup() {
  // put your setup code here, to run once:
  Wire.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
}