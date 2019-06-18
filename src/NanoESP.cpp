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

#include "NanoESP.h"
 
 // Send a message to the serial output
void NanoESP::message(const char *level, const char *message)
{
#ifndef ESP_NO_SERIAL_OUTPUT
    if (strcmp(this->LVL_DEBUG, level) == 0)
    {
#ifdef ESP_DEBUG
        Serial.print("[N_ESP/DEBUG] ");
        Serial.println(message);
#endif
    }
    else
    {
        Serial.print("[N_ESP/");
        Serial.print(level);
        Serial.print("] ");
        Serial.println(message);
    }
#endif
}

// Start the SoftwareSerial and resets the ESP
void NanoESP::setupESP()
{
    this->esp.begin(NANO_ESP_BAUDRATE);
    this->esp.setTimeout(5000);
    if (sendCommand("AT+RST", "ready"))
    {
        message(this->LVL_INFO, "RESET OK");
    }
}

// Sends a command to the ESP
String NanoESP::sendCommand(const char *command)
{
    message(this->LVL_DEBUG, command);
    this->esp.println(command);
    return this->esp.readString();
}

// Sends a command to the ESP and checks for the respond
bool NanoESP::sendCommand(const char *command, const char *respond)
{
    this->esp.println(command);
    if (this->esp.findUntil((char *)respond, (char *)"ERROR"))
    {
        message(LVL_DEBUG, command);
        return true;
    }
    else
    {
        message(LVL_ERROR, command);
        return false;
    }
}

// Setup the accesspoint with a SSID and Password
bool NanoESP::setupAP(String ssid, String pass)
{
    bool success = true;

    success &= sendCommand("AT+CWMODE=2", "OK");
    success &= sendCommand(String("AT+CWSAP=\"" + ssid + "\",\"" + pass + "\",5,0").c_str(), "OK");

    return success;
}

// Setup the UDP Server
bool NanoESP::setupUDP(String ip, String outputPort, String inputPort)
{
    boolean success = true;

    success &= sendCommand("AT+CIPMODE=0", "OK");
    success &= sendCommand("AT+CIPMUX=0", "OK");
    success &= sendCommand(String("AT+CIPSTART=\"UDP\",\"" + ip + "\"," + outputPort + "," + inputPort).c_str(), "OK");

    return success;
}

// Returns a reference to the SoftwareSerial
SoftwareSerial& NanoESP::getESPSerial()
{
    return this->esp;
}