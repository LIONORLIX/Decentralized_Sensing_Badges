# Project Deliverables

Organized: 2026-11-14

This directory collects the project files that are ready for delivery, further fabrication, or development reference. The original files remain in their original project locations; the files here are organized copies for submission and archiving.

## Project video

[Watch the project demonstration video on YouTube](https://www.youtube.com/watch?v=fy3ZHzJYr6A&t=1s)

## Directory structure

```text
project_deliverables/
|-- 01_final_code/                 # Arduino code used by the final prototype
|-- 02_model_sources/              # Rhino modeling source files (.3dm)
|-- 03_3d_print_files/             # Printable meshes for import into a slicer (.stl)
`-- 04_development_reference_code/ # Earlier experiments and course sketches
```

## 1. Final delivery code

| File | Purpose | Original path |
|---|---|---|
| [`badge_send/badge_send.ino`](01_final_code/badge_send/badge_send.ino) | Badge node: reads environmental data from an MQ135 sensor and sends it with WifiEspNow | `final/LO2/code/badge_send/badge_send.ino` |
| [`station_recieve/station_recieve.ino`](01_final_code/station_recieve/station_recieve.ino) | Station node: receives the data and displays it with a 14-pixel NeoPixel ring | `final/LO2/code/station_recieve/station_recieve.ino` |

Identical copies of both sketches also exist under `final/xiang_li_23009641/LO2/code/`. Only one copy of each sketch is included here to avoid maintaining duplicate delivery files.

### Before flashing

1. Open each `.ino` file from its own folder in the Arduino IDE.
2. Install and select the ESP8266 board package and the required libraries: `WifiEspNow`, `MQ135`, and `Adafruit NeoPixel`.
3. Update the `PEER` address in both sketches with the MAC address of the other device.
4. The badge node uses GPIO13 (D7) for the MQ135 sensor and GPIO14 (D5) for the button. The station node uses GPIO13 (D7) for the NeoPixel ring and GPIO14 (D5) for the button.

## 2. Modeling source files

| File | Intended use | Original path |
|---|---|---|
| [`base.3dm`](02_model_sources/base.3dm) | Base or enclosure model | `fab/base.3dm` |
| [`Badge.3dm`](02_model_sources/Badge.3dm) | Main badge model | `fab/Badge.3dm` |
| [`Badge_test.3dm`](02_model_sources/Badge_test.3dm) | Test badge model | `fab/Badge_test.3dm` |

## 3. 3D printing files

| File | Status | Original path |
|---|---|---|
| [`Badge_Circle.stl`](03_3d_print_files/Badge_Circle.stl) | Badge mesh ready for slicer import | `fab/Badge_Circle.stl` |
| [`Badge_test.stl`](03_3d_print_files/Badge_test.stl) | Test mesh ready for slicer import | `fab/Badge_test.stl` |

No `.gcode`, `.gco`, `.3mf`, or other exported slicing output was found in the project. The two `.stl` files are pre-slicing meshes. Import them into the slicer for the target printer, set the material, layer height, infill, and supports, then export the printer-specific G-code.

## 4. Development reference code

`04_development_reference_code/` contains 12 non-final Arduino sketches copied with their original relative paths:

- `Board_communication/`: DHT11, LCD, button, and communication experiments.
- `ESP32_communication/`: ESP-NOW master/slave communication and MAC-address experiments.
- `ESP_LED/`: humidity-driven LED experiment.
- `pcomp_project/`: ESP8266 DHT11 and LCD tests.
- `sketch_oct15a/`, `week3course/`, and `week4course/`: earlier course exercises.

These files are kept for development history and reference. Use the sketches in `01_final_code/` for the final prototype.

## File verification

SHA-256 checksums for the final code copies:

- `badge_send.ino`: `7c22144852c1237cb0d9e559f76a4d15563ca796be1d363895b33ffbaebf3d73`
- `station_recieve.ino`: `e0494491e1d788a0714a15f89c3bdcfa646450b8b786f74467a2fc49b9015606`
