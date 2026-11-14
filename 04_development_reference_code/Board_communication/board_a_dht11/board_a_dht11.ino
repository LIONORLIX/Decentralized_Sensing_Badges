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

// Include the DHT11 library for interfacing with the sensor.
#include <DHT11.h>
#include <MQ135.h>

#include <WifiEspNow.h>
#if defined(ARDUINO_ARCH_ESP8266)
#include <ESP8266WiFi.h>
#elif defined(ARDUINO_ARCH_ESP32)
#include <WiFi.h>
#endif

DHT11 dht11(27);

#define PIN_MQ135 32

MQ135 mq135_sensor(PIN_MQ135);

// The recipient MAC address. It must be modified for each device.
static uint8_t PEER[]{0xAA, 0x48, 0xFA, 0xCE, 0x7A, 0x7C};

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
}

void
loop()
{
  char msg[60];
  // Attempt to read the temperature and humidity values from the DHT11 sensor.
  int temperature = dht11.readTemperature();
  // If using ESP32 or ESP8266 (xtensa architecture), uncomment the delay below.
  // This ensures stable readings when calling methods consecutively.
  delay(50);
  int humidity = dht11.readHumidity();

  float rzero = mq135_sensor.getRZero();
  float correctedRZero = mq135_sensor.getCorrectedRZero(temperature, humidity);
  float resistance = mq135_sensor.getResistance();
  float ppm = mq135_sensor.getPPM();
  float correctedPPM = mq135_sensor.getCorrectedPPM(temperature, humidity);

  Serial.print("MQ135 RZero: ");
  Serial.print(rzero);
  Serial.print("\t Corrected RZero: ");
  Serial.print(correctedRZero);
  Serial.print("\t Resistance: ");
  Serial.print(resistance);
  Serial.print("\t PPM: ");
  Serial.print(ppm);
  Serial.print("\t Corrected PPM: ");
  Serial.print(correctedPPM);
  Serial.println("ppm");

  if (temperature != DHT11::ERROR_CHECKSUM && temperature != DHT11::ERROR_TIMEOUT &&
        humidity != DHT11::ERROR_CHECKSUM && humidity != DHT11::ERROR_TIMEOUT)
  {
      Serial.print("Temperature: ");
      Serial.print(temperature);
      Serial.println(" °C");
      int len1 = snprintf(msg, sizeof(msg), "temperature",
                     WiFi.softAPmacAddress().c_str(), millis());
          WifiEspNow.send(PEER, reinterpret_cast<const uint8_t*>(msg), len1);

      Serial.print("Humidity: ");
      Serial.print(humidity);
      Serial.println(" %");
      int len2 = snprintf(msg, sizeof(msg), "humidity",
                     WiFi.softAPmacAddress().c_str(), millis());
      WifiEspNow.send(PEER, reinterpret_cast<const uint8_t*>(msg), len2);
  }else
  {
      if (temperature == DHT11::ERROR_TIMEOUT || temperature == DHT11::ERROR_CHECKSUM)
      {
          // Serial.print("Temperature Reading Error: ");
          // Serial.println(DHT11::getErrorString(temperature));
          int len3 = snprintf(msg, sizeof(msg), "error",
                     WiFi.softAPmacAddress().c_str(), millis());
          WifiEspNow.send(PEER, reinterpret_cast<const uint8_t*>(msg), len3);
      }
      if (humidity == DHT11::ERROR_TIMEOUT || humidity == DHT11::ERROR_CHECKSUM)
      {
          int len4 = snprintf(msg, sizeof(msg), "error",
                     WiFi.softAPmacAddress().c_str(), millis());
          WifiEspNow.send(PEER, reinterpret_cast<const uint8_t*>(msg), len4);
      }
  }

  int len5 = snprintf(msg, sizeof(msg), "hello ESP-NOW from %s at %lu",
                     WiFi.softAPmacAddress().c_str(), millis());
  WifiEspNow.send(PEER, reinterpret_cast<const uint8_t*>(msg), len5);
  delay(1000);
}
