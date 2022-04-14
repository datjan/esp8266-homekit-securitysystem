/*
 * my_accessory.c
 * Define the accessory in C language using the Macro in characteristics.h
 *
 *  Created on: 2020-05-15
 *      Author: Mixiaoxiao (Wang Bin)
 */

#include <homekit/homekit.h>
#include <homekit/characteristics.h>

// Called to identify this accessory. See HAP section 6.7.6 Identify Routine
// Generally this is called when paired successfully or click the "Identify Accessory" button in Home APP.
void my_accessory_identify(homekit_value_t _value) {
  printf("accessory identify\n");
}

// For SECURITY_SYSTEM,

/**
 Defines that the accessory contains a security system.
 
 Required Characteristics:
 - SECURITY_SYSTEM_CURRENT_STATE
 - SECURITY_SYSTEM_TARGET_STATE
 
 Optional Characteristics:
 - NAME
 - STATUS_FAULT
 - STATUS_TAMPERED
 - SECURITY_SYSTEM_ALARM_TYPE
 */

// (required) format: float; 0-4
homekit_characteristic_t cha_current_state = HOMEKIT_CHARACTERISTIC_(SECURITY_SYSTEM_CURRENT_STATE, 0);

// (required) format: float; 0-3
homekit_characteristic_t cha_target_state = HOMEKIT_CHARACTERISTIC_(SECURITY_SYSTEM_TARGET_STATE, 0);

// (optional) format: string; HAP section 9.62; max length 64
homekit_characteristic_t cha_name = HOMEKIT_CHARACTERISTIC_(NAME, "Security System");


homekit_accessory_t *accessories[] = {
    HOMEKIT_ACCESSORY(.id=1, .category=homekit_accessory_category_security_system, .services=(homekit_service_t*[]) {
        HOMEKIT_SERVICE(ACCESSORY_INFORMATION, .characteristics=(homekit_characteristic_t*[]) {
            HOMEKIT_CHARACTERISTIC(NAME, "ESP Security System"),
            HOMEKIT_CHARACTERISTIC(MANUFACTURER, "DATJAN"),
            HOMEKIT_CHARACTERISTIC(SERIAL_NUMBER, "datjan.esp32.001"),
            HOMEKIT_CHARACTERISTIC(MODEL, "esp8266-homekit-securitysystem"),
            HOMEKIT_CHARACTERISTIC(FIRMWARE_REVISION, "1.0"),
            HOMEKIT_CHARACTERISTIC(IDENTIFY, my_accessory_identify),
            NULL
        }),
        HOMEKIT_SERVICE(SECURITY_SYSTEM, .primary=true, .characteristics=(homekit_characteristic_t*[]) {
            &cha_current_state,
            &cha_target_state,
            &cha_name,//optional
      //&cha_status_active,//optional
      //&cha_status_fault,//optional
      //&cha_status_tampered,//optional
      //&cha_status_low_battery,//optional
            NULL
        }),
    // Add this HOMEKIT_SERVICE if you has a HUMIDITY_SENSOR together
    /*
        HOMEKIT_SERVICE(HUMIDITY_SENSOR, .characteristics=(homekit_characteristic_t*[]) {
            HOMEKIT_CHARACTERISTIC(NAME, "Humidity Sensor"),
            &cha_humidity,
            NULL
        }),*/
        NULL
    }),
    NULL
};

homekit_server_config_t config = {
    .accessories = accessories,
    .password = "123-55-222" // SETUP Homekit Code
};
