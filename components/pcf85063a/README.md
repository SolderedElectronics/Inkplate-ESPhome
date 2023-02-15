```
esphome:
  name: inkplate6plus
  libraries:
    - "Wire"

external_components:
  - source: 
      type: git
      url: https://github.com/SolderedElectronics/Inkplate-ESPhome
      ref: main
    components: [pcf85063a]

esp32:
  board: esp32dev
  framework:
    type: arduino

# Enable logging
logger:

# Enable Home Assistant API
api:
  encryption:
    key: "CCGlHGSh8qflAUKc+1h88IvsC1msonqsrzrAtJMzDnc="

ota:
  password: "8ad3af65095957dbc49d98725149ecc1"

wifi:
  ssid: !secret wifi_ssid
  password: !secret wifi_password

  # Enable fallback hotspot (captive portal) in case wifi connection fails
  ap:
    ssid: "Inkplate6Plus Fallback Hotspot"
    password: "2x45XHQ2CZve"

captive_portal:

# example configuration:
pcf85063a:
  id: pcf85063a_test
  address: 0x51  

i2c:
  sda: 21
  scl: 22
```