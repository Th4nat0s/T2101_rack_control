#include <Arduino.h>
#include "ATtinySerialOut.h"
#include <avr/sleep.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// Debug serial sur PB2 - 115200bps  (D2)
// Data wire is plugged into port 2 on the Arduino

#define ONE_WIRE_BUS 7  // Attention pinout étrange
#define LED 10
#define MOTOR 4
#define SLEEP_LOOP 9  // 9 = 8 Sec,   8 = 4 Sec

// ATMEL ATTINY84 / ARDUINO
//
//                              +-\/-+
//                        VCC  1|    |14  GND
//                (D  0)  PB0  2|    |13  PA0  (D 10)        AREF
//                (D  1)  PB1  3|    |12  PA1  (D  9)
//                (D 11)  PB3  4|    |11  PA2  (D  8)
//  PWM T0  INT0  (D  2)  PB2  5|    |10  PA3  (D  7)
//  PWM T0        (D  3)  PA7  6|    |9   PA4  (D  6)
//  PWM T1        (D  4)  PA6  7|    |8   PA5  (D  5)        PWM T1
//                              +----+

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(ONE_WIRE_BUS);

// oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);

#define adc_disable() (ADCSRA &= ~(1 << ADEN)) // Disable ADC to save a lot of current consumption.

void setup(void)
{
  #ifdef __PLATFORMIO_BUILD_DEBUG__
  initTXPin();
  Serial.println("Start attiny");
  #endif
  // Start up the library
  // Configure les ports et le sensor
  pinMode(LED, OUTPUT);
  // Blink on start 3x
  digitalWrite(LED,HIGH);
  delay(250);
  digitalWrite(LED,LOW);
  delay(250);
  digitalWrite(LED,HIGH);
  delay(250);
  digitalWrite(LED,LOW);
  delay(250);
  digitalWrite(LED,HIGH);
  delay(250);
  digitalWrite(LED,LOW);
  delay(250);


  //pinMode(ONE_WIRE_BUS, OUTPUT);
  //digitalWrite(ONE_WIRE_BUS, HIGH); // Activate Pull Up
  sensors.begin();

  adc_disable();                       // ADC uses ~320uA so disable it until we need it.

  // Setup PWM Timer1
  //  Default 250Hz
  // TCCR1B = TCCR1B & (B11111000 | B00000001); //this changes the PWM frequency to 16kHz

  // Launch motor
  // Doit tourner a plein régime pour lancer les moteurs.

  digitalWrite(LED,HIGH);
  pinMode(MOTOR, OUTPUT);
  analogWrite(MOTOR, 255);
  delay(2000);
}


/*
 * Main function, get and show the temperature, then sleep
 */
void loop(void)
{
  digitalWrite(LED,LOW);

  #ifdef __PLATFORMIO_BUILD_DEBUG__
  Serial.print("Check Temperature..");
  #endif

  sensors.requestTemperatures(); // Send the command to get temperatures

  #ifdef __PLATFORMIO_BUILD_DEBUG__
  Serial.println(".Done");
  #endif

  // get temperature
  float tempC = sensors.getTempCByIndex(0);

  // Check if reading was successful
  if(tempC != DEVICE_DISCONNECTED_C) 
  {
    #ifdef __PLATFORMIO_BUILD_DEBUG__
    Serial.print("Temperature is: ");
    Serial.println(tempC);
    #endif
  }
  else
  {
    #ifdef __PLATFORMIO_BUILD_DEBUG__
    Serial.println("Error: Could not read temperature data");
    #endif
    // si on perd le capteur, ventilo a fond.
    tempC = 100;
  }



  // Setup ulow, low, medium, high
   if (tempC < 34){
     analogWrite(MOTOR, 48);
    #ifdef __PLATFORMIO_BUILD_DEBUG__
    Serial.println("Motor: 48");
    #endif
  }
  else if (tempC > 35 and tempC < 39){
    analogWrite(MOTOR, 64);
    #ifdef __PLATFORMIO_BUILD_DEBUG__
    Serial.println("Motor: 64");
    #endif
  }
  else if (tempC > 40 and tempC <44){
    analogWrite(MOTOR, 128);
    #ifdef __PLATFORMIO_BUILD_DEBUG__
    Serial.println("Motor: 128");
    #endif
  }
  else if (tempC > 45){
    analogWrite(MOTOR, 255);
    #ifdef __PLATFORMIO_BUILD_DEBUG__
    Serial.println("Motor: 255");
    #endif
}

  // Alume la led après traitement
  digitalWrite(LED,HIGH);

  // Wait 30 Sec
  delay(30000);
}
