// The MIT License (MIT)
// 
// Copyright (c) 2018 Sebastian Knackstedt
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

//Includes the NanoESP library
#include "NanoESP.h"

// Defines the output pins
#define PIN_RED   3
#define PIN_GREEN 5
#define PIN_BLUE  6

// Creates a NanoESP objects
NanoESP esp;

void setup()
{
  // Starts the serial output [Baudrate=19200]
  Serial.begin(19200); 

  // Sets ouput pins
  pinMode(PIN_RED,   OUTPUT);
  pinMode(PIN_GREEN, OUTPUT);
  pinMode(PIN_BLUE,  OUTPUT);

  // NanoESP functions [enable AccessPoint and UDP]
  esp.setupESP();
  esp.setupAP(); // Can also be used with parameter: esp.setupAP("TestWifi", "MyPassword"); [See NanoESP.h for details]
  esp.setupUDP(); // Can also be used with parameter: esp.setupUDP("192.168.4.7", "128", "129"); [See NanoESP.h for details]
}

void loop()
{
  // Scans for new packages
  if (esp.getESPSerial().available())
  {
    // All packages which starts '+IPD,'
    if (esp.getESPSerial().find( (char*) "+IPD,"))
    {
      // and have 'led' in their payload [package format = 'led{red}{green}{blue}\n' e.g.= led255255255\n or led000000000\n] (\n is the Ascii line-feed character (0x0A))
      if (esp.getESPSerial().find( (char*) "led"))
      {
        //Read package data
        String command = esp.getESPSerial().readStringUntil('\n');
        
        //Extract the color value
        int red   = command.substring(0, 3).toInt();
        int green = command.substring(3, 6).toInt();
        int blue  = command.substring(6, 9).toInt();

        //Write the value to the output pins [0 = full-off, 255 = full-on]
        analogWrite(PIN_RED,   red  );
        analogWrite(PIN_GREEN, green);
        analogWrite(PIN_BLUE,  blue );
      }
      else
      {
        // Sends error packet
        if (esp.sendCommand("AT+CIPSEND=19", ">"))
        {
          esp.sendCommand("Wrong UDP Command", "OK");
        }
      }
    }
  }
  if(Serial.available()){
    esp.getESPSerial().write(Serial.read());
  }
  if(esp.getESPSerial().available()){
    Serial.write(esp.getESPSerial().read());
  }
}