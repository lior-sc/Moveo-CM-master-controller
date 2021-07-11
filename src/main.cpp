#include <Arduino.h>
#include <Wire.h>
#include "ros.h"

#define DEBUG     true

byte J12_buffer[4] = { 0 };
byte J345_buffer[6] = { 0 };
float Joints[5];

void clear_serial_buffer(int iterations)
{
  for(int i=0;i<iterations;i++)
  {
    Serial.read();
  }
  return;
}
float getSerialfloat(String str)
{
  Serial.println(str);
  clear_serial_buffer(100);
  while(!Serial.available())
  {
    // nothing
  }
  float temp=Serial.parseFloat();
  return temp;
}
void get_serial_joints(float val[0])
{
  Serial.println("enter joint values in radian:");
  val[0] = getSerialfloat("J1:");
  val[1]  = getSerialfloat("J2:");
  val[2]  = getSerialfloat("J3:");
  val[3]  = getSerialfloat("J4:");
  val[4]  = getSerialfloat("J5:");
}

void send_joint_positions()
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


// loop functions
void serial_test_loop()
{
  get_serial_joints(&Joints[0]);
  Serial.println("sending joint positions");
  send_joint_positions();
  Serial.println("positions sent\n");
}


void setup() 
{
  if(DEBUG)
  {
    Serial.begin(115200);
  }

  Wire.begin();
}

void loop() 
{
  serial_test_loop();
}