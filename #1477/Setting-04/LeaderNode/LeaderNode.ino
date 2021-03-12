#include "arduino_secrets.h"

// Enable debug prints to serial monitor
#define MY_DEBUG
// Enable support for I_DEBUG messages.
#define MY_SPECIAL_DEBUG

#define MY_NODE_ID 20

// Enable and select radio type attached
#define MY_RADIO_RF24
// Set transmission channel
#define MY_RF24_CHANNEL SECRET_RF24_CHANNEL

// Enabled repeater feature for this node
#define MY_REPEATER_FEATURE

#define DEBUG_PIN1 A0
#define DEBUG_PIN2 A1
#define DEBUG_PIN3 A2

#include <MySensors.h>

#define FOLLOWER_NODE 21

#define MY_CHILD_SENSOR_ID 0

MyMessage msg(MY_CHILD_SENSOR_ID, V_STATUS);

void presentation()
{
	sendSketchInfo("MySensors Leader Node", __DATE__);
	present(MY_CHILD_SENSOR_ID, S_BINARY, "Receiver");
}

void receive(const MyMessage& message)
{
	static uint8_t lastValue = 0;

	// We only expect one type of message from controller. But we better check anyway.
	if (message.type == V_STATUS) {
		if (message.sender == FOLLOWER_NODE) {

			uint8_t dataSize = message.getLength();
			uint8_t* data = message.getCustom();
			uint8_t thisValue = data[dataSize - 1];

			uint8_t actualValue = thisValue;
			if (actualValue == 0) {
				actualValue = 16;
			}

			if (actualValue - 1 != lastValue) {
				Serial.print("Wrong order - delta: ");
				int delta = thisValue - lastValue;
				delta = delta < 0 ? delta + 16 : delta;
				Serial.print(delta);
				Serial.print(" last: ");
				Serial.print(lastValue);
				Serial.print(" this: ");
				Serial.println(thisValue);

				// Pin debug: wrong order received
				digitalWrite(DEBUG_PIN3, HIGH);
				delay(5);
				digitalWrite(DEBUG_PIN3, LOW);
			}
			lastValue = thisValue;

			// Pin debug: message received
			digitalWrite(DEBUG_PIN2, HIGH);
			delay(5);
			digitalWrite(DEBUG_PIN2, LOW);
		}
	}
}

bool sendMessage()
{
	static const bool requestEcho = true;
	static const uint8_t payloadSize = MAX_PAYLOAD_SIZE;
	static uint8_t data[payloadSize] = { 0 };
	static uint8_t sendValue = 0;

	data[payloadSize - 1] = sendValue;
	sendValue++;
	if (sendValue == 16) {
		sendValue = 0;
	}
	msg.set(data, payloadSize);
	msg.setDestination(FOLLOWER_NODE);

	// Pin debug: send message
	digitalWrite(DEBUG_PIN2, HIGH);
	// Send message and request echo message from receiver
	bool success = send(msg, requestEcho);
	digitalWrite(DEBUG_PIN2, LOW);

	return success;
}

void setup()
{
	// Activate debug pins
	pinMode(DEBUG_PIN1, OUTPUT);
	pinMode(DEBUG_PIN2, OUTPUT);
	pinMode(DEBUG_PIN3, OUTPUT);
}

void loop()
{
	static uint32_t lastMillis = 0;

	// Pin debug: loop start
	digitalWrite(DEBUG_PIN1, HIGH);

	if (millis() - lastMillis > 1000) {

		// Send 16 messages to follower in a row
		for (size_t i = 0; i < 16; i++) {
			bool success = sendMessage();
			if (!success) {
				// Pin debug: send failed
				digitalWrite(DEBUG_PIN3, HIGH);
				delay(3);
				digitalWrite(DEBUG_PIN3, LOW);
			}
		}
		lastMillis = millis();
	}
	// Pin debug: loop end
	digitalWrite(DEBUG_PIN1, LOW);
}
