/*****************************************************************************/
// Function: Get the accelemeter of the x/y/z axis.
// Hardware: Grove - 3-Axis Digital Accelerometer(±1.5g)
// Arduino IDE: Arduino-1.0
// Author: Frankie.Chu
// Date: Jan 10,2013
// Version: v0.9b
// by www.seeedstudio.com
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
//
/*******************************************************************************/
#include <Wire.h>
#include <MMA7660.h>
#include <MemoryFree.h>

float refax,refay,refaz;
int blueLed=10;
int greenLed=12;
int redLed=11;
MMA7660 accelemeter;
int i=0;
float GSENSIBILITY=0.2;
float BSENSIBILITY=0.4;
float RSENSIBILITY=0.6;

int8_t x;
int8_t y;
int8_t z;
float ax,ay,az;

void setup()
{
  accelemeter.init();
  Serial.begin(9600);

  calculateMean(&refax,&refay,&refaz);
  Serial.println("mean of of X/Y/Z: ");
  Serial.print(refax);
  Serial.println(" g");
  Serial.print(refay);
  Serial.println(" g");
  Serial.print(refaz);
  Serial.println(" g");
  Serial.println("==============");
  //
  pinMode(blueLed, OUTPUT);
  pinMode(greenLed, OUTPUT);
  pinMode(redLed, OUTPUT);

  
}

void loop()
{

  /*accelemeter.getXYZ(&x,&y,&z);
  Serial.print("x = ");
  Serial.println(x);
  Serial.print("y = ");
  Serial.println(y);
  Serial.print("z = ");
  Serial.println(z);
  */
  Serial.println("====");
  accelemeter.getAcceleration(&ax,&ay,&az);
  Serial.println(i);
  /*Serial.println("accleration of X/Y/Z: ");
  Serial.print(ax);
  Serial.println(" g");
  Serial.print(ay);
  Serial.println(" g");
  Serial.print(az);
  Serial.println(" g");
  Serial.println("*************");*/
  delay(500);
  i++;
  if (checkSensibility(ay,refay,RSENSIBILITY)==1)
    lightLed(redLed);
  else {
    if (checkSensibility(ay,refay,BSENSIBILITY)==1)
      lightLed(blueLed);
    else {
      if (checkSensibility(ay,refay,GSENSIBILITY)==1)
        lightLed(greenLed);
     } 
  }
  Serial.print("freeMemory()=");
  Serial.println(freeMemory());
  if (checkSensibility(ay,refay,GSENSIBILITY)==1){
    Serial.print("normal y=");
    Serial.print(refay);
    Serial.print("current y=");
    Serial.println(ay);
  }
}

void calculateMean(float *ax,float *ay,float *az){
  int ITER=20;
  int DELAY=500;
  *ax=0;*ay=0;*az=0;
  float tempax,tempay,tempaz;
  for(int i=0;i<ITER;i++){
    accelemeter.getAcceleration(&tempax,&tempay,&tempaz);
    *ax=*ax+tempax;
    *ay=*ay+tempay;
    *az=*az+tempaz;
    delay(DELAY);
  }
  *ax=*ax/ITER;
  *ay=*ay/ITER;
  *az=*az/ITER;
}

int checkSensibility(float value,float refvalue,float sensibility){
  value=abs(value);
  refvalue=abs(refvalue);
  float bmin=refvalue-refvalue*sensibility;
  float bmax=refvalue+refvalue*sensibility;
  /*Serial.print("is ");
  Serial.print(bmin);
  Serial.print(" < ");
  Serial.print(value);
  Serial.print(" < ");
  Serial.println(bmax);*/
  if ((value<bmin) or (value>bmax)){
    return 1;
  }
  return 0;
}

void lightLed(int color){
  digitalWrite(color, HIGH);
  delay(500);
  digitalWrite(color, LOW);
}

