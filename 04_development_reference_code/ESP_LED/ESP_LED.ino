
#include <DHT11.h>

// Create an instance of the DHT11 class.
// - For Arduino: Connect the sensor to Digital I/O Pin 2.
// - For ESP32: Connect the sensor to pin GPIO2 or P2.
// - For ESP8266: Connect the sensor to GPIO2 or D4.
DHT11 dht11(32);
/*
 LEDC Software Fade

 This example shows how to software fade LED
 using the ledcWrite function.

 Code adapted from original Arduino Fade example:
 https://www.arduino.cc/en/Tutorial/Fade

 This example code is in the public domain.
 */

// use first channel of 16 channels (started from zero)
#define LEDC_CHANNEL_0     0

// use 12 bit precission for LEDC timer
#define LEDC_TIMER_12_BIT  12

// use 5000 Hz as a LEDC base frequency
#define LEDC_BASE_FREQ     5000

// fade LED PIN (replace with LED_BUILTIN constant for built-in LED)
#define LED_PIN            27

int brightness = 0;    // how bright the LED is
int fadeAmount = 5;    // how many points to fade the LED by

// Arduino like analogWrite
// value has to be between 0 and valueMax
void ledcAnalogWrite(uint8_t channel, uint32_t value, uint32_t valueMax = 255) {
  // calculate duty, 4095 from 2 ^ 12 - 1
  uint32_t duty = (4095 / valueMax) * min(value, valueMax);

  // write duty to LEDC
  ledcWrite(channel, duty);
}

void setup() {
  // Setup timer and attach timer to a led pin
  ledcSetup(LEDC_CHANNEL_0, LEDC_BASE_FREQ, LEDC_TIMER_12_BIT);
  ledcAttachPin(LED_PIN, LEDC_CHANNEL_0);
  Serial.begin(9600);
}

void loop() {
  // set the brightness on LEDC channel 0

  int humidity = dht11.readHumidity();
  
  long humidity_light = map(humidity,60,80,0,255);
  if (humidity_light<0){
    humidity_light=0;
  }else if(humidity_light>255){
    humidity_light=255;
  }

  ledcAnalogWrite(LEDC_CHANNEL_0, humidity_light);
  Serial.println(humidity_light);

  // // change the brightness for next time through the loop:
  // brightness = brightness + fadeAmount;

  // // reverse the direction of the fading at the ends of the fade:
  // if (brightness <= 0 || brightness >= 255) {
  //   fadeAmount = -fadeAmount;
  // }
  // // wait for 30 milliseconds to see the dimming effect
  delay(30);
}
