#include <Arduino.h>
#include <stdint.h>
#include <Wire.h>
#include "../lib/therm.h"
#include "../lib/SteinhartHart.h"
#include "../lib/config.h"

// i2c address
#define ADDRESS 0x08

// thermistor pins
#define THERM_PIN_OUTSIDE 0
#define THERM_PIN_TANK 1

// resistance of the thermistor resistor
#define THERM_OHM_OUTSIDE 10000
#define THERM_OHM_TANK 10000

volatile therm_s therm;
volatile config_s config;
SteinhartHart outsideTemp;
SteinhartHart tankTemp;

void postI2c()
{
    therm.outside = outsideTemp.getTemp(getOhm(THERM_PIN_OUTSIDE, THERM_OHM_OUTSIDE));
    therm.tank = tankTemp.getTemp(getOhm(THERM_PIN_TANK, THERM_OHM_TANK));
    Wire.write((uint8_t *)&therm, sizeof(therm_s));
}

void getI2c(int byteNum)
{
    Wire.readBytes((uint8_t *)&config, sizeof(config_s));
}

float getOhm(uint32_t pin, uint32_t refOhm)
{
    float raw = analogRead(pin);

    // convert to ohm
    raw = (1032 / raw) - 1;
    return refOhm / raw;
}

void setup()
{
    Wire.begin(ADDRESS);
    Wire.onRequest(postI2c);
    Wire.onReceive(getI2c);

    // The beta coefficient of the thermistor (usually 3000-4000)
    // static definition for now
    // TODO: allow configurable params

    outsideTemp = SteinhartHart::bParam(3950U);
    tankTemp = SteinhartHart::bParam(3950U);
}

void loop()
{
}