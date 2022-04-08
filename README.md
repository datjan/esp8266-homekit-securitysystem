# esp8266-homekit-securitysystem
This is a native HomeKit security system with D1 mini based on potential free contacts.

## Hardware
The following hardware is required:
```
- D1 mini (ESP8266)
- Resistors 10kOhm
- Potential free contacts (Like buttons)
```

Connection:
D1 mini -> Potential free contact
```
GND -> Potential free contact
D? -> Potential free contact
D? -> 10kOhm -> 3V3
```

D1 = homekit state away

D2 = homekit state triggered

D5 = homekit state night

D6 = homekit state stay


![alt text](https://github.com/datjan/esp8266-homekit-securitysystem/blob/main/connection%20schema.png?raw=true)

## Development
This sketch is for following development environment
```
Arduino
```

Following libraries are required
```
https://github.com/datjan/Arduino-HomeKit-ESP8266 (fork from Mixiaoxiao/Arduino-HomeKit-ESP8266:master)
```

## Setup
Setup my_accessory.c:
```
.password = "123-55-222". // Homekit Code
```

Setup wifi_info.h
```
const char *ssid = "xxx"; // SETUP Wlan ssid
const char *password = "xxx"; // SETUP Wlan password
```

## Upload to device
Following files needs to be uploaded to the ESP8266 (D1 mini)
```
esp8266-homekit-securitysystem.ino
my_accessory.c
wifi_info.h
```

## Add device to Homekit
The device can be added to homekit like every other homekit device.
