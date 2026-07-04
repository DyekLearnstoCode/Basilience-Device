#include "SensorModule.h"

SensorModule sensors;

// =====================================================
// SETUP
// =====================================================

void setup()
{

    Serial.begin(115200);

    Serial.println();
    Serial.println("==================================");
    Serial.println(" BASILIENCE STARTING...");
    Serial.println("==================================");

    sensors.begin();

}

// =====================================================
// LOOP
// =====================================================

void loop()
{

    //--------------------------------------------------
    // UPDATE ALL SENSORS
    //--------------------------------------------------

    sensors.update();

    //--------------------------------------------------
    // GET SENSOR DATA
    //--------------------------------------------------

    SensorData sensor = sensors.getData();

    //--------------------------------------------------
    // PRINT
    //--------------------------------------------------

    sensors.print();

    //--------------------------------------------------
    // FIREBASE
    //--------------------------------------------------

    /*
        TODO

        Firebase.setFloat("temperature",sensor.waterTemperature);

        Firebase.setFloat("ph",sensor.pH);

        Firebase.setFloat("ec",sensor.ec);

        Firebase.setFloat("ppm",sensor.ppm);
    */

    //--------------------------------------------------
    // FOGGER
    //--------------------------------------------------

    /*
        TODO

        Automatic Fogger Logic

    */

    //--------------------------------------------------
    // FAN
    //--------------------------------------------------

    /*
        TODO

        Automatic Fan Logic

    */

    //--------------------------------------------------
    // NUTRIENT DOSING
    //--------------------------------------------------

    /*
        TODO

        if(sensor.ec < TARGET_EC)

        {

            Dose Nutrient

        }

    */

    //--------------------------------------------------
    // PH DOSING
    //--------------------------------------------------

    /*
        TODO

        if(sensor.pH < TARGET_PH_LOW)

        {

            Dose PH UP

        }

        if(sensor.pH > TARGET_PH_HIGH)

        {

            Dose PH DOWN

        }

    */

    delay(2000);

}
