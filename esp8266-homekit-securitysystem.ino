
/*  WEMOS D1 Mini
                     ______________________________
                    |   L T L T L T L T L T L T    |
                    |                              |
                 RST|                             1|TX HSer
                  A0|                             3|RX HSer
                  D0|16                           5|D1
                  D5|14                           4|D2
                  D6|12                    10kPUP_0|D3
RX SSer/HSer swap D7|13                LED_10kPUP_2|D4
TX SSer/HSer swap D8|15                            |GND
                 3V3|__                            |5V
                       |                           |
                       |___________________________|


D1 mini -> Potential free contact (state away)
GND -> Potential free contact
D1 -> Potential free contact
D1 -> 10kOhm -> 3V3

D1 mini -> Potential free contact (state triggered)
GND -> Potential free contact
D2 -> Potential free contact
D2 -> 10kOhm -> 3V3

D1 mini -> Potential free contact (state night)
GND -> Potential free contact
D5 -> Potential free contact
D5 -> 10kOhm -> 3V3

D1 mini -> Potential free contact (state stay)
GND -> Potential free contact
D6 -> Potential free contact
D6 -> 10kOhm -> 3V3


Possible states: Stay=0 / Away=1 / Night=2 / Disarmed=3 / Triggered=4
*/

#include <Arduino.h>
#include <arduino_homekit_server.h>    // https://github.com/datjan/Arduino-HomeKit-ESP8266
#include "wifi_info.h"

// Pins
int pinStatusAway = D1;
int pinStatusTriggered = D2;
int pinStatusNight = D5;
int pinStatusStay = D6;

// Security Status
uint8_t securitystate = 3; // Default: 3 Disarm (Stay=0 / Away=1 / Night=2 / Disarmed=3 / Triggered=4)


#define LOG_D(fmt, ...)   printf_P(PSTR(fmt "\n") , ##__VA_ARGS__);

void setup() {
  Serial.begin(115200);

  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);

  // Temporarily set pins to output and HIGH
  pinMode(pinStatusAway, OUTPUT); 
  pinMode(pinStatusTriggered, OUTPUT); 
  pinMode(pinStatusNight, OUTPUT); 
  pinMode(pinStatusStay, OUTPUT); 
  digitalWrite(pinStatusAway, HIGH); 
  digitalWrite(pinStatusTriggered, HIGH); 
  digitalWrite(pinStatusNight, HIGH); 
  digitalWrite(pinStatusStay, HIGH); 
  // Pins to Input
  pinMode(pinStatusAway, INPUT); // Default state HIGH
  pinMode(pinStatusTriggered, INPUT); // Default state HIGH
  pinMode(pinStatusNight, INPUT); // Default state HIGH
  pinMode(pinStatusStay, INPUT); // Default state HIGH

  
  wifi_connect(); // in wifi_info.h
  
  // homekit_storage_reset(); // to remove the previous HomeKit pairing storage when you first run this new HomeKit example
  my_homekit_setup();
  
}

void loop() {
  my_homekit_loop();
  delay(1000);
}

//==============================
// Homekit setup and loop
//==============================

// access your homekit characteristics defined in my_accessory.c
extern "C" homekit_server_config_t config;
extern "C" homekit_characteristic_t cha_current_state;
extern "C" homekit_characteristic_t cha_target_state;

static uint32_t next_heap_millis = 0;
static uint32_t next_report_millis = 0;

void my_homekit_setup() {
  arduino_homekit_setup(&config);
}

void my_homekit_loop() {
  arduino_homekit_loop();
  const uint32_t t = millis();
  if (t > next_report_millis) {
    // report sensor values every 10 seconds
    next_report_millis = t + 10 * 1000;
    my_homekit_report();
  }
  if (t > next_heap_millis) {
    // show heap info every 5 seconds
    next_heap_millis = t + 5 * 1000;
    LOG_D("Free heap: %d, HomeKit clients: %d",
        ESP.getFreeHeap(), arduino_homekit_connected_clients_count());

  }
}

void my_homekit_report() {

  if (digitalRead(pinStatusTriggered) == LOW)
    {
       // Triggered - Ausgeloest
       securitystate = 4;
    }
  else if (digitalRead(pinStatusAway) == LOW)
    {
       // Away - Abwesend
       securitystate = 1;
    }
  else if (digitalRead(pinStatusNight) == LOW)
    {
       // Night - Nacht
       securitystate = 2;
    }
  else if (digitalRead(pinStatusStay) == LOW)
    {
       // Stay - Zuhause
       securitystate = 0;
    }
  else
    {
        // Disarmed - Aus
        securitystate = 3;
    }


  LOG_D("State: %u - %s", securitystate, state_text(securitystate));
      
  // If state triggered
  if (securitystate == 4)
    {
      // If triggered no target state only current state update!
      LOG_D("State triggered");
      // Get currentstate
      cha_current_state.value.uint8_value = securitystate;
      LOG_D("Set current state: %u - %s", securitystate, state_text(securitystate));
      homekit_characteristic_notify(&cha_current_state, cha_current_state.value);
    }
  else
    {   
      LOG_D("State normal");
      // Send targetstate
      LOG_D("Set target state: %u - %s", securitystate, state_text(securitystate));
      cha_target_state.value.uint8_value = securitystate;
      homekit_characteristic_notify(&cha_target_state, cha_target_state.value);
    
      delay(1000);
      
      // Send currentstate
      LOG_D("Set current state: %u - %s", securitystate, state_text(securitystate));
      cha_current_state.value.uint8_value = securitystate;
      homekit_characteristic_notify(&cha_current_state, cha_current_state.value);
    }

  // Status LED
  digitalWrite(LED_BUILTIN, LOW);   // turn the LED on
  delay(200);                       // wait 
  digitalWrite(LED_BUILTIN, HIGH);  // turn the LED off

}

char *state_text(uint8_t state) {
  char *state_name = "";
  if (state == 0)
    {
      state_name = "Stay / Zuhause (Pin D6 closed)";
    }
  else if (state == 1)
    {
      state_name = "Away / Abwesend (Pin D1 closed)";
    }
  else if (state == 2)
    {
      state_name = "Night / Nacht (Pin D5 closed)";
    }
  else if (state == 3)
    {
      state_name = "Disarmed / Aus (No pin closed)";
    }
  else if (state == 4)
    {
      state_name = "Triggered / Ausgeloest (Pin D2 closed)";
    }
  return state_name;
}
