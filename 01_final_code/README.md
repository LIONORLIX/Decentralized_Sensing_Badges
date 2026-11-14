# Final Delivery Code

This directory contains the two Arduino sketches used by the final prototype:

- `badge_send/badge_send.ino`: reads MQ135 data and sends it with WifiEspNow.
- `station_recieve/station_recieve.ino`: receives the data and drives a 14-pixel NeoPixel ring.

Before flashing, update the `PEER` address in both sketches with the MAC address of the other device and install the ESP8266, WifiEspNow, MQ135, and Adafruit NeoPixel dependencies. See the parent [`README.md`](../README.md) for the complete file map.
