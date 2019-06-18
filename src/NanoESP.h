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

#include <Arduino.h>
#include <SoftwareSerial.h>

// Software serial rx/tx pins
#define NANO_ESP_RX 11
#define NANO_ESP_TX 12

// Enable debug printing
#define ESP_DEBUG

// Can be defined to disable serial output
// #define ESP_NO_SERIAL_OUTPUT

// The baudrate for the software serial
#define NANO_ESP_BAUDRATE 19200

class NanoESP
{
  // Private functions
  private:
    SoftwareSerial esp = SoftwareSerial(NANO_ESP_RX, NANO_ESP_TX);

    const char *LVL_DEBUG = "Debug";
    const char *LVL_INFO = "Info";
    const char *LVL_WARNING = "Warning";
    const char *LVL_ERROR = "Error";

    void message(const char *level, const char *message);

  // Public functions
  public:
    // Sends a command to the esp and waits for the respond
    bool sendCommand(const char *command, const char *respond);

    // Setup the esp (THIS HAS TO BE CALLED BEFORE ALL OTHER FUNCTIONS)
    void setupESP();

    // Sends a command to the esp
    String sendCommand(const char *command);

    // Setup the access point
    bool setupAP(String ssid = "NanoESP", String pass = "");

    // Setup the UDP server
    bool setupUDP(String ip = "192.168.4.2", String ouputPort = "90", String inputPort = "91");

    SoftwareSerial& getESPSerial();
};