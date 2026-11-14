/**
 * @file
 *
 * EspNowUnicast.ino demonstrates how to transmit unicast ESP-NOW messages with @c WifiEspNow .
 * You need two ESP8266 or ESP32 devices to run this example.
 *
 * Unicast communication requires the sender to specify the MAC address of the recipient.
 * Thus, you must modify this program for each device.
 *
 * The recommended workflow is:
 * @li 1. Flash the program onto device A.
 * @li 2. Run the program on device A, look at serial console for its MAC address.
 * @li 3. Copy the MAC address of device A, paste it in the @c PEER variable below.
 * @li 4. Flash the program that contains A's MAC address onto device B.
 * @li 5. Run the program on device A, look at serial console for its MAC address.
 * @li 6. Copy the MAC address of device B, paste it in the @c PEER variable below.
 * @li 7. Flash the program that contains B's MAC address onto device A.
 */

#include <WifiEspNow.h>
#if defined(ARDUINO_ARCH_ESP8266)
#include <ESP8266WiFi.h>
#elif defined(ARDUINO_ARCH_ESP32)
#include <WiFi.h>
#endif

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif
#define PIN  13 // On Trinket or Gemma, suggest changing this to 1
#define NUMPIXELS 10 // Popular NeoPixel ring size
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

int BUTTON_PIN = 14; 

// #define BUTTON_PIN D5

// The recipient MAC address. It must be modified for each device.
static uint8_t PEER[]{0x02, 0x00, 0x00, 0x45, 0x53, 0x50};

void
printReceivedMessage(const uint8_t mac[WIFIESPNOW_ALEN], const uint8_t* buf, size_t count,
                     void* arg)
{
  Serial.printf("Message from %02X:%02X:%02X:%02X:%02X:%02X\n", mac[0], mac[1], mac[2], mac[3],
                mac[4], mac[5]);
  for (int i = 0; i < static_cast<int>(count); ++i) {
    Serial.print(static_cast<char>(buf[i]));
  }
  Serial.println();
}

void
setup()
{
  Serial.begin(115200);
  Serial.println();

  WiFi.persistent(false);
  WiFi.mode(WIFI_AP);
  WiFi.disconnect();
  WiFi.softAP("ESPNOW", nullptr, 3);
  WiFi.softAPdisconnect(false);
  // WiFi must be powered on to use ESP-NOW unicast.
  // It could be either AP or STA mode, and does not have to be connected.
  // For best results, ensure both devices are using the same WiFi channel.

  Serial.print("MAC address of this node is ");
  Serial.println(WiFi.softAPmacAddress());

  uint8_t mac[6];
  WiFi.softAPmacAddress(mac);
  Serial.println();
  Serial.println("You can paste the following into the program for the other device:");
  Serial.printf("static uint8_t PEER[]{0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X};\n", mac[0],
                mac[1], mac[2], mac[3], mac[4], mac[5]);
  Serial.println();

  bool ok = WifiEspNow.begin();
  if (!ok) {
    Serial.println("WifiEspNow.begin() failed");
    ESP.restart();
  }

  WifiEspNow.onReceive(printReceivedMessage, nullptr);

  ok = WifiEspNow.addPeer(PEER);
  if (!ok) {
    Serial.println("WifiEspNow.addPeer() failed");
    ESP.restart();
  }

  pinMode(BUTTON_PIN,INPUT);

  #if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif

  pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
}

void
loop()
{
  // digitalWrite(LED_BUILTIN_PIN, LOW);
  pixels.clear(); // Set all pixel colors to 'off'

  // The first NeoPixel in a strand is #0, second is 1, all the way up
  // to the count of pixels minus one.
  
  // char msg[60];
  // int len = snprintf(msg, sizeof(msg), "hello ESP-NOW from %s at %lu",
  //                    WiFi.softAPmacAddress().c_str(), millis());
  // WifiEspNow.send(PEER, reinterpret_cast<const uint8_t*>(msg), len);
  // delay(1000);
  if (digitalRead(BUTTON_PIN)){
    // digitalWrite(LED_BUILTIN_PIN, HIGH);
    Serial.println("hi");
    for(int i=0; i<NUMPIXELS; i++) { // For each pixel...
    // pixels.Color() takes RGB values, from 0,0,0 up to 255,255,255
    // Here we're using a moderately bright green color:
    pixels.setPixelColor(i, pixels.Color(150, 0, map(i,0,13,0,255)));

    // delay(500);
    }
    
  }else{
    // digitalWrite(LED_BUILTIN_PIN, LOW);
    if (digitalRead(BUTTON_PIN)){
    // digitalWrite(LED_BUILTIN_PIN, HIGH);
      Serial.println("hi");
    for(int i=0; i<NUMPIXELS; i++) { // For each pixel...
    // pixels.Color() takes RGB values, from 0,0,0 up to 255,255,255
    // Here we're using a moderately bright green color:
      pixels.setPixelColor(i, pixels.Color(0, 0, 0));

    }

    }
    // delay(5000);
  }
  pixels.show();   // Send the updated pixel colors to the hardware.
}
