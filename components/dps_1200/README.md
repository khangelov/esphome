# DPS-1200 ESPHome External Component

This ESPHome external component provides I2C access to HP server power supplies such as the DPS-1200, allowing monitoring of:

- Input Voltage (grid_voltage)
- Output Voltage (output_voltage)
- Input Current (IIN)
- Output Current (IOUT)
- Input Power (input_power
- Output Power (output_power)
- Internal Temperature (internal_temperature)
- Fan Speed (fan_rpm:)

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
    components: [dps_1200]


i2c:
  sda: GPIO6
  scl: GPIO5
  scan: true
  id: bus_a
  frequency: 100kHz


sensor:
  - platform: dps_1200
    update_interval: 10s
    grid_voltage:
      name: "Grid Voltage"
    grid_current:
      name: "Grid Current"
    output_voltage:
      name: "Output Voltage"
    output_current:
      name: "Output Current"
    internal_temperature:
      name: "Internal Temperature"
    fan_rpm:
      name: "Fan RPM"
    input_power:
      name: "Input Power"
    output_power:
      name: "Output Power"
```
