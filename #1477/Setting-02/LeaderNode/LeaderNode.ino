#include "arduino_secrets.h"

// Enable debug prints to serial monitor
#define MY_DEBUG
// Enable support for I_DEBUG messages.
#define MY_SPECIAL_DEBUG

#define MY_NODE_ID 17

// Enable and select radio type attached
#define MY_RADIO_RF24
// Set transmission channel
#define MY_RF24_CHANNEL SECRET_RF24_CHANNEL
// Set auto retry count
#define MY_RF24_AUTO_RETRY_COUNT 0

// Use extended transport handler
#define MY_TRANSPORT_HAL_SEND_HANDLER
//#define MY_TRANSPORT_HAL_RECEIVE_HANDLER

#define DEBUG_PIN1 A0
#define DEBUG_PIN2 A1
#define DEBUG_PIN3 A2

#include <MySensors.h>
#include "MyTransportHandler.h"

#define SYNC_PIN 8 // Wire pin to connect with the follower node to synchronise message sending

const uint32_t sendDelay = 30 + 900;

#define MY_CHILD_SENSOR_ID 0

MyMessage msg(MY_CHILD_SENSOR_ID, V_STATUS);

void presentation()
{
    sendSketchInfo("MySensors Leader Node", __DATE__);
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

    // Send sync signal to follower
    digitalWrite(SYNC_PIN, HIGH);
    delayMicroseconds(200);
    digitalWrite(SYNC_PIN, LOW);

    // Give follower a little time
    delayMicroseconds(sendDelay);

    // Pin debug: send message
    digitalWrite(DEBUG_PIN2, HIGH);
    bool success = send(msg);
    digitalWrite(DEBUG_PIN2, LOW);

    return success;
}

void setup()
{
    // Activate synchronisation pin
    pinMode(SYNC_PIN, OUTPUT);
    digitalWrite(SYNC_PIN, LOW);

    // Activate debug pins
    pinMode(DEBUG_PIN1, OUTPUT);
    pinMode(DEBUG_PIN2, OUTPUT);
    pinMode(DEBUG_PIN3, OUTPUT);

    randomSeed(analogRead(A4)); // Analog pin A4 shall be not connected
}

void loop()
{
    static uint32_t lastMillis = 0;

    // Pin debug: loop start
    digitalWrite(DEBUG_PIN1, HIGH);

    if (millis() - lastMillis > 500) {
        bool success = sendMessage();
        if (!success) {
            // Pin debug: send failed
            digitalWrite(DEBUG_PIN3, HIGH);
            delay(3);
            digitalWrite(DEBUG_PIN3, LOW);
        }
        lastMillis = millis();
    }
    // Pin debug: loop end
    digitalWrite(DEBUG_PIN1, LOW);
}
