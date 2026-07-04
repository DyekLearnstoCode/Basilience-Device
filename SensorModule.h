#ifndef SENSOR_MODULE_H
#define SENSOR_MODULE_H

#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// ======================================================
// BASILIENCE SENSOR MODULE
// ESP32
// ------------------------------------------------------
// Handles:
// • DS18B20
// • pH Sensor
// • TDS / EC Sensor
// ======================================================

// ======================================================
// PIN DEFINITIONS
// ======================================================

// Analog Inputs
#define PH_PIN              34
#define TDS_PIN             35

// DS18B20
#define ONE_WIRE_BUS        4

// ======================================================
// TDS CALIBRATION
// ======================================================

// Your calibrated values

const int TDS_ADC_ZERO = 20;
const int TDS_ADC_REF  = 344;

const float EC_REFERENCE = 1.413f;

// ======================================================
// PH CALIBRATION
// ======================================================

// Your calibrated values

const int PH_ADC_686 = 822;
const int PH_ADC_401 = 977;

const float PH_686 = 6.86f;
const float PH_401 = 4.01f;

// ======================================================
// SENSOR FILTERING
// ======================================================

const uint8_t SENSOR_SAMPLES = 30;

// ======================================================
// STATUS ENUMS
// ======================================================

enum ECStatus
{
    EC_LOW,
    EC_OPTIMAL,
    EC_HIGH,
    EC_CRITICAL
};

enum PHStatus
{
    PH_ACIDIC,
    PH_OPTIMAL,
    PH_ALKALINE
};

// ======================================================
// SENSOR DATA
// ======================================================

struct SensorData
{
    float waterTemperature;

    float pH;

    float ec;

    float ppm;

    ECStatus ecStatus;

    PHStatus phStatus;
};

// ======================================================
// CLASS
// ======================================================

class SensorModule
{

private:

    OneWire* oneWire;
    DallasTemperature* ds18b20;

    SensorData data;

    //--------------------------------------------------

    int readAverageADC(uint8_t pin);

    float adcToPH(int adc);

    float adcToEC(int adc);

    void updatePH();

    void updateEC();

    void updateTemperature();

    void updateStatus();

public:

    SensorModule();
    ~SensorModule();

    //--------------------------------------------------

    void begin();

    void update();

    void print();

    //--------------------------------------------------

    SensorData getData();

};

#endif
