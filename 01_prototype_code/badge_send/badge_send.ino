// The code is written based on:
// Example of WifiEspNow Library
// Example of MQ135 Library

// including ESP WiFi communication library and set up
#include <WifiEspNow.h>
#if defined(ARDUINO_ARCH_ESP8266)
#include <ESP8266WiFi.h>
#elif defined(ARDUINO_ARCH_ESP32)
#include <WiFi.h>
#endif
// Set the recipient MAC address. It must be modified for each device.
static uint8_t PEER[]{0xAA, 0x48, 0xFA, 0xCE, 0x7A, 0x7C};

// set button pin, the button controls on/off of the LED ring
#define BTN_PIN 14 // ESP8266 GPIO14 pin (D5)

// including MQ135 library and set up
#include <MQ135.h>
#define DATA_PIN 13 // ESP8266 GPIO13 pin (D7)
MQ135 mq135_sensor(DATA_PIN);
float temperature = 21.0; // Assume current temperature
float humidity = 25.0; // Assume current humidity

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
  // check PEER ESP board
  ok = WifiEspNow.addPeer(PEER);
  if (!ok) {
    Serial.println("WifiEspNow.addPeer() failed");
    ESP.restart();
  }
  // set up button pin mode
  pinMode(BTN_PIN,INPUT);
}

void loop() {

  float data = mq135_sensor.getCorrectedRZero(temperature, humidity);

  Serial.println(data);
  // recieve and send message through WiFi
  char msg[60];
  // here set the message to be sent
  int len = snprintf(msg, sizeof(msg), "%f", data); 
  WifiEspNow.send(PEER, reinterpret_cast<const uint8_t*>(msg), len);// directly send the value
  delay(1000);

}
