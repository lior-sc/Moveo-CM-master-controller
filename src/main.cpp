#include <Arduino.h>
#include <Wire.h>
#include "ros.h"

#define DEBUG               true
#define CMD_TYPE_MOVE       1

uint8_t J12_buffer[5] = { 0 };
uint8_t J345_buffer[7] = { 0 };
int16_t joint_buffer[5] = { 0 };
double Joints[5];
double msg_test_angle=0.5; // rad

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
void blink_led(int times, unsigned long int dt)
{
  for(int i=0;i<times;i++)
  {
    digitalWrite(13,HIGH);
    delay(dt);
    digitalWrite(13,LOW);
    delay(dt);
  }
}
void get_serial_joints(double val[0])
{
  Serial.println("enter joint values in radian:");
  val[0]  = (double)getSerialfloat("J1:");
  val[1]  = (double)getSerialfloat("J2:");
  val[2]  = (double)getSerialfloat("J3:");
  val[3]  = (double)getSerialfloat("J4:");
  val[4]  = (double)getSerialfloat("J5:");
}

void send_joint_positions()
{
  // the function arranges the buffers in Big Endian configuration (MSB first)

  uint8_t _cmd_type = CMD_TYPE_MOVE;

  for (int i=0; i<5; i++)
  {
    joint_buffer[i] = (int16_t)(Joints[i] * 1000);
  }

  // Joints 12 controller cmd
  J12_buffer[0]=_cmd_type;

  for(int i=0; i<2; i++)
  {
    J12_buffer[2*i + 1] = joint_buffer[i] >> 8;     // MSB
    J12_buffer[2*i + 2] = joint_buffer[i];          // LSB
  }
  
  // Joints 345 controller cmd
  J345_buffer[0]=_cmd_type;

  for(int i=0; i<3; i++)
  {
    J345_buffer[2*i + 1] = joint_buffer[i+2] >> 8;  // MSB
    J345_buffer[2*i + 2] = joint_buffer[i+2];       // LSB
  }

  //send buffers via I2C
  Wire.beginTransmission(2);
  Wire.write(J12_buffer, 5);
  Wire.endTransmission();

  Wire.beginTransmission(3);
  Wire.write(J345_buffer, 7);
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
void send_message_test_loop()
{
  msg_test_angle*=-1;
  blink_led(2,500);
  for(int i=0;i<5;i++)
  {
    Joints[i]=msg_test_angle;
  }
  send_joint_positions();
  delay(1000);
}


void setup() 
{
  pinMode(13,OUTPUT);
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