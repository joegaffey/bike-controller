/*
  USB HID Bike Controller
  
  This sketch reads an analog voltage from pin A0, maps the 
  reading to a joystick axis value, and sends it to the computer.
  
  This requires an Arduino board capable of acting as a Human Interface
  Device (HID), such as a Leonardo, Micro, or Due. It will not work
  on an Arduino Uno.
  
  You must install the "Arduino HID Project" library from the
  Arduino IDE's Library Manager to use this sketch.
*/

// Include the Joystick library
#include <Joystick.h>

// Define the analog pin to read from
const int analogPin = A0; 

// Define the voltage reference of the Arduino board (Pro Micro here)
const float referenceVoltage = 3.3;

// The maximum analog reading is 1023 (10-bit ADC)
const float maxAnalogValue = 1023.0;

// Initialize the joystick with one axis (Rx-axis) and a range of 0-1023.
// The X-axis has been disabled and the Rx-axis has been enabled.
Joystick_ joystick(JOYSTICK_DEFAULT_REPORT_ID, JOYSTICK_TYPE_GAMEPAD,
                   0, 0,  // Number of buttons and hats
                   false, // X-axis disabled
                   false, // Y-axis disabled
                   false, // Z-axis disabled
                   true, // Rx-axis enabled
                   false, // Ry-axis disabled
                   false); // Rz-axis disabled


void setup() {
  // Begin joystick communication
  joystick.begin();
  
  // Configure pin A1 as an output pin and set its voltage to HIGH
  pinMode(A1, OUTPUT);
  digitalWrite(A1, HIGH);
}

void loop() {
  // Read the analog value from the specified pin
  int analogValue = analogRead(analogPin);

  // --- MODIFIED CODE ---

  // Define the target voltage range
  const float minVoltage = 1.6;
  const float maxVoltage = 2.4;
  
  // Convert the voltage range into the corresponding analog reading range
  const int minAnalog = (minVoltage / referenceVoltage) * maxAnalogValue;
  const int maxAnalog = (maxVoltage / referenceVoltage) * maxAnalogValue;

  // Use the constrain function to clamp the analog value to the desired range
  int clampedAnalogValue = constrain(analogValue, minAnalog, maxAnalog);
  
  // Map the clamped analog value (minAnalog to maxAnalog) to the joystick's
  // full axis range (0 to 1023)
  int joystickValue = map(clampedAnalogValue, minAnalog, maxAnalog, 0, 1023);

  // Map the analog value (0-1023) directly to the joystick's Rx-axis
  joystick.setRxAxis(joystickValue);
  
  // --- END MODIFIED CODE ---

  // Wait a short period to prevent flooding the USB bus with data
  delay(10); 
}
