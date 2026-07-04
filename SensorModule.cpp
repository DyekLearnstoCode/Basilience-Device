#include "SensorModule.h"

// ======================================================
// CONSTRUCTOR
// ======================================================

SensorModule::SensorModule()
{
    oneWire = new OneWire(ONE_WIRE_BUS);
    ds18b20 = new DallasTemperature(oneWire);

    data.waterTemperature = 0.0;
    data.pH = 0.0;
    data.ec = 0.0;
    data.ppm = 0.0;

    data.ecStatus = EC_LOW;
    data.phStatus = PH_OPTIMAL;
}

SensorModule::~SensorModule()
{
    delete ds18b20;
    delete oneWire;
}

// ======================================================
// INITIALIZE
// ======================================================

void SensorModule::begin()
{
    ds18b20->begin();
}

// ======================================================
// READ AVERAGE ADC
// ======================================================

int SensorModule::readAverageADC(uint8_t pin)
{
    long total = 0;

    for(int i=0;i<SENSOR_SAMPLES;i++)
    {
        total += analogRead(pin);
        delay(10);
    }

    return total / SENSOR_SAMPLES;
}

// ======================================================
// ADC -> PH
// ======================================================

float SensorModule::adcToPH(int adc)
{
    float slope =
        (PH_401 - PH_686) /
        (float)(PH_ADC_401 - PH_ADC_686);

    return slope * (adc - PH_ADC_686) + PH_686;
}

// ======================================================
// ADC -> EC
// ======================================================

float SensorModule::adcToEC(int adc)
{
    float ec =
        (float)(adc - TDS_ADC_ZERO);

    ec *= EC_REFERENCE;

    ec /= (TDS_ADC_REF - TDS_ADC_ZERO);

    if(ec < 0)
        ec = 0;

    return ec;
}

// ======================================================
// TEMPERATURE
// ======================================================

void SensorModule::updateTemperature()
{
    ds18b20->requestTemperatures();

    data.waterTemperature =
        ds18b20->getTempCByIndex(0);
}

// ======================================================
// PH
// ======================================================

void SensorModule::updatePH()
{
    int adc = readAverageADC(PH_PIN);

    data.pH = adcToPH(adc);
}

// ======================================================
// EC
// ======================================================

void SensorModule::updateEC()
{
    int adc = readAverageADC(TDS_PIN);

    data.ec = adcToEC(adc);

    data.ppm = data.ec * 500.0;
}

// ======================================================
// STATUS
// ======================================================

void SensorModule::updateStatus()
{

    //--------------------------------------------------
    // EC
    //--------------------------------------------------

    if(data.ec < 0.8)
    {
        data.ecStatus = EC_LOW;
    }
    else if(data.ec <= 1.6)
    {
        data.ecStatus = EC_OPTIMAL;
    }
    else if(data.ec <= 2.2)
    {
        data.ecStatus = EC_HIGH;
    }
    else
    {
        data.ecStatus = EC_CRITICAL;
    }

    //--------------------------------------------------
    // PH
    //--------------------------------------------------

    if(data.pH < 5.8)
    {
        data.phStatus = PH_ACIDIC;
    }
    else if(data.pH <= 6.3)
    {
        data.phStatus = PH_OPTIMAL;
    }
    else
    {
        data.phStatus = PH_ALKALINE;
    }

}

// ======================================================
// UPDATE ALL
// ======================================================

void SensorModule::update()
{
    updateTemperature();

    updatePH();

    updateEC();

    updateStatus();
}

// ======================================================
// PRINT
// ======================================================

void SensorModule::print()
{
    Serial.println();
    Serial.println("========================================");
    Serial.println("         BASILIENCE SENSOR DATA");
    Serial.println("========================================");

    Serial.print("Water Temp : ");
    Serial.print(data.waterTemperature,2);
    Serial.println(" °C");

    //--------------------------------------

    Serial.print("pH         : ");
    Serial.println(data.pH,2);

    Serial.print("pH Status  : ");

    switch(data.phStatus)
    {
        case PH_ACIDIC:
            Serial.println("ACIDIC");
            break;

        case PH_OPTIMAL:
            Serial.println("OPTIMAL");
            break;

        case PH_ALKALINE:
            Serial.println("ALKALINE");
            break;
    }

    //--------------------------------------

    Serial.print("EC         : ");
    Serial.print(data.ec,3);
    Serial.println(" mS/cm");

    Serial.print("PPM        : ");
    Serial.println(data.ppm,0);

    Serial.print("EC Status  : ");

    switch(data.ecStatus)
    {
        case EC_LOW:
            Serial.println("LOW");
            break;

        case EC_OPTIMAL:
            Serial.println("OPTIMAL");
            break;

        case EC_HIGH:
            Serial.println("HIGH");
            break;

        case EC_CRITICAL:
            Serial.println("CRITICAL");
            break;
    }

    Serial.println("========================================");
}

// ======================================================
// GET DATA
// ======================================================

SensorData SensorModule::getData()
{
    return data;
}
