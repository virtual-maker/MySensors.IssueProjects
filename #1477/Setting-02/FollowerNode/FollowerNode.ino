#include "arduino_secrets.h"

// Enable debug prints to serial monitor
#define MY_DEBUG
// Enable support for I_DEBUG messages.
#define MY_SPECIAL_DEBUG

#define MY_NODE_ID 18

// Enable and select radio type attached
#define MY_RADIO_RF24
// Set transmission channel
#define MY_RF24_CHANNEL SECRET_RF24_CHANNEL

#include <MySensors.h>

const uint32_t sendDelay = 0;

#define SUCCESS_MESSAGE 5 //!< Each nth message shall be successful to avoid transport layer reset

#define SYNC_PIN 8 // Wire pin to connect with the leader node to synchronise both nodes

#define DEBUG_PIN1 A0
#define DEBUG_PIN2 A1
#define DEBUG_PIN3 A2

#define MY_CHILD_SENSOR_ID 0

MyMessage msg(MY_CHILD_SENSOR_ID, V_STATUS);

void presentation()
{
    sendSketchInfo("MySensors Follower Node", __DATE__);
    present(MY_CHILD_SENSOR_ID, S_BINARY, "Receiver");
}

void receive(const MyMessage& message)
{
}

bool sendMessage()
{
    static const uint8_t payloadSize = MAX_PAYLOAD_SIZE;
    static uint8_t data[payloadSize] = { 0 };

    data[payloadSize - 1] = 42;
    msg.set(data, payloadSize);

    delay(sendDelay);

    // Pin debug: send message
    digitalWrite(DEBUG_PIN2, HIGH);
    bool success = send(msg);
    digitalWrite(DEBUG_PIN2, LOW);
    
    return success;
}

void setup()
{
    // Activate synchronisation pin
    pinMode(SYNC_PIN, INPUT);

    // Activate debug pins
    pinMode(DEBUG_PIN1, OUTPUT);
    pinMode(DEBUG_PIN2, OUTPUT);
    pinMode(DEBUG_PIN3, OUTPUT);
}

void loop()
{
    static bool lastSync = LOW;
    static uint8_t messageCounter = 0;

    // Pin debug: loop start
    digitalWrite(DEBUG_PIN1, HIGH);

    bool thisSync = digitalRead(SYNC_PIN);
    if (thisSync && !lastSync) {
        // timeout counter to detect HW issues
        uint16_t timeout = 0xFFFF;
        // wait for falling edge
        while (digitalRead(SYNC_PIN) && timeout--) {
        }
        // Let be sendMessage() successful each SUCCESS_MESSAGE
        if (++messageCounter >= SUCCESS_MESSAGE) {
            messageCounter = 0;
            // Delay sendMessage to avoid collision
            delay(100);
        }
        bool success = sendMessage();
        if (!success) {
            // Pin debug: send failed
            digitalWrite(DEBUG_PIN3, HIGH);
            delay(3);
            digitalWrite(DEBUG_PIN3, LOW);
        }
    }
    lastSync = thisSync;

    // Pin debug: loop end
    digitalWrite(DEBUG_PIN1, LOW);
}
