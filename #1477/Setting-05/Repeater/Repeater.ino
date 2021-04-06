#include "arduino_secrets.h"

// Enable debug prints to serial monitor
//#define MY_DEBUG
// Enable support for I_DEBUG messages.
#define MY_SPECIAL_DEBUG

// My own node id
#define MY_NODE_ID 30
// Force to connect to repeater with node id 0
#define MY_PARENT_NODE_ID (0)
#define MY_PARENT_NODE_IS_STATIC

// Enables and select radio type (if attached)
#define MY_RADIO_RF24
// Set transmission channel
#define MY_RF24_CHANNEL SECRET_RF24_CHANNEL

// Enabled repeater feature for this node
#define MY_REPEATER_FEATURE

#include <MySensors.h>

void presentation()
{
    //Send the sensor node sketch version information to the gateway
    sendSketchInfo("MySensors Repeater", __DATE__);
}

void setup()
{
}

void loop()
{
}
