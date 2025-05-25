# DPS-1200 ESPHome External Component

This ESPHome external component provides I2C access to HP server power supplies such as the DPS-1200, allowing monitoring of:

- Input Voltage (VIN)
- Output Voltage (VOUT)
- Input Current (IIN)
- Output Current (IOUT)
- Input Power (PIN)
- Output Power (POUT)
- Internal Temperature
- Fan Speed

## Hardware Requirements

- HP DPS-1200 or compatible PSU with I2C mod
- ESP32 or ESP8266
- I2C lines connected with appropriate pull-up resistors

## Installation

Copy this folder to your ESPHome config or use `external_components` with GitHub.

## Example Configuration

```yaml
external_components:
  - source:
      type: git
      url: https://github.com/khangelov/esphome
      ref: main
      path: components/dps_1200

i2c:
  sda: GPIO21
  scl: GPIO22
  scan: true

sensor:
  - platform: dps_1200
    update_interval: 10s
    vin:
      name: "VIN"
    vout:
      name: "VOUT"
    iin:
      name: "IIN"
    iout:
      name: "IOUT"
    pin:
      name: "Power In"
    pout:
      name: "Power Out"
    temp:
      name: "Temperature"
    fan:
      name: "Fan Speed"
```
