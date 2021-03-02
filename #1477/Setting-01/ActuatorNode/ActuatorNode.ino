#include "arduino_secrets.h"

// Enable debug prints to serial monitor
#define MY_DEBUG
// Enable support for I_DEBUG messages.
#define MY_SPECIAL_DEBUG

#define MY_NODE_ID 16

// Enable and select radio type attached
#define MY_RADIO_RF24
// Set transmission channel
#define MY_RF24_CHANNEL SECRET_RF24_CHANNEL

#include <MySensors.h>

const size_t packetCount = 8;

#define MY_CHILD_SENSOR_ID 0

void presentation()
{
    sendSketchInfo("MySensors Node", __DATE__);
    present(MY_CHILD_SENSOR_ID, S_BINARY, "Receiver");
}

void receive(const MyMessage& message)
{
    static uint8_t lastValue = 0;

    // We only expect one type of message from controller. But we better check anyway.
    if (message.type == V_STATUS) {
        if (message.sensor == MY_CHILD_SENSOR_ID) {

            //uint8_t thisValue = message.getByte();
            uint8_t dataSize = message.getLength();
            uint8_t* data = (uint8_t*)message.getCustom();
            uint8_t thisValue = data[dataSize - 1];

            int actualValue = thisValue;
            if (actualValue == 0) {
                actualValue = packetCount;
            }

            if (actualValue - 1 != lastValue) {
                Serial.print("Wrong order - delta: ");
                int delta = thisValue - lastValue;
                delta = delta < 0 ? delta + packetCount : delta;
                Serial.print(delta);
                Serial.print(" last: ");
                Serial.print(lastValue);
                Serial.print(" this: ");
                Serial.println(thisValue);
            }
            lastValue = thisValue;
        }
    }
}

void setup()
{
}

void loop()
{
}
