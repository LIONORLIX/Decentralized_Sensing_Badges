// The code is written based on:
// Example of WifiEspNow Library
// Example of NEOPixel Library

// Reference Code: 
// https://forum.arduino.cc/t/strncpy-strange-results/114568
// https://forum.arduino.cc/t/converting-char-to-int/101204/5
// https://forum.arduino.cc/t/float-double-atof-arduino/96463


// including ESP WiFi communication library and set up
#include <WifiEspNow.h>
#if defined(ARDUINO_ARCH_ESP8266)
#include <ESP8266WiFi.h>
#elif defined(ARDUINO_ARCH_ESP32)
#include <WiFi.h>
#endif

// Set the recipient MAC address. It must be modified for each device.
float recieved_value = 0;
static uint8_t PEER[]{0xBE, 0xFF, 0x4D, 0x5F, 0xBA, 0x70};

// process received message
// char type[10]; // set up 2 value to store key message, one is data type
char value[3]; // set up 2 value to store key message, one is data value
void printReceivedMessage(const uint8_t mac[WIFIESPNOW_ALEN], const uint8_t* buf, size_t count, void* arg){
  // set a char with enough storage
  char received_char[count];
  strncpy(received_char, (const char*)buf, count);
  // convert to int for visualization
  recieved_value = atof(received_char);
}

// set button pin, the button controls on/off of the LED ring
#define BTN_PIN 14 // ESP8266 GPIO14 pin (D5)

// include NeoPixel Library
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif
#define PIN  13 // On Trinket or Gemma, suggest changing this to 1
#define NUMPIXELS 14 // Popular NeoPixel ring size
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  // set up Serial
  Serial.begin(115200);

  // set up WiFi
  Serial.println();
  WiFi.persistent(false);
  WiFi.mode(WIFI_AP);
  WiFi.disconnect();
  WiFi.softAP("ESPNOW", nullptr, 3);
  WiFi.softAPdisconnect(false);
  // get MAC address of this device
  Serial.print("MAC address of this node is ");
  Serial.println(WiFi.softAPmacAddress());
  uint8_t mac[6];
  WiFi.softAPmacAddress(mac);
  Serial.println();
  Serial.println("You can paste the following into the program for the other device:");
  Serial.printf("static uint8_t PEER[]{0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X};\n", mac[0],
                mac[1], mac[2], mac[3], mac[4], mac[5]); // Here print the MAC address of this device
  Serial.println();
  // check WiFi connection
  bool ok = WifiEspNow.begin();
  if (!ok) {
    Serial.println("WifiEspNow.begin() failed");
    ESP.restart();
  }
  WifiEspNow.onReceive(printReceivedMessage, nullptr);
  // check PEER ESP board
  ok = WifiEspNow.addPeer(PEER);
  if (!ok) {
    Serial.println("WifiEspNow.addPeer() failed");
    ESP.restart();
  }

  // set up button pin mode
  pinMode(BTN_PIN,INPUT);

  // Neopixel set up
  #if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
    clock_prescale_set(clock_div_1);
  #endif
  pixels.begin();

}

void loop() {
  // Set all pixel colors to 'off'
  pixels.clear(); 
  if (digitalRead(BTN_PIN)==HIGH){
    // turn recieved value to light numbers
    Serial.println(recieved_value);
    int light_count = map(int(recieved_value), 230, 270, 1, NUMPIXELS);
    for(int i=0; i<light_count; i++) {
      // visualize with a gradient color
      pixels.setPixelColor(i, pixels.Color(150, 0, map(i,0,NUMPIXELS-1,0,255)));
    }
  }else{
    for(int i=0; i<NUMPIXELS; i++) {
      // turn off the light
      pixels.setPixelColor(i, pixels.Color(0, 0, 0));
    }
  }
  pixels.show();// Send the updated pixel colors to the hardware.
}
