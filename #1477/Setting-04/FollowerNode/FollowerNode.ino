#include "arduino_secrets.h"

// Enable debug prints to serial monitor
//#define MY_DEBUG
// Enable support for I_DEBUG messages.
#define MY_SPECIAL_DEBUG

// I'm the follower node
#define MY_NODE_ID 21
// Force to connect to repeater with ID 20 (the leader node)
#define MY_PARENT_NODE_ID 20
// Disable fall back if parent node fails
#define MY_PARENT_NODE_IS_STATIC

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

// After 4 failed messages the next one shall be sent successfully to avoid reset of transport layer
#define MAX_FAILED_MESSAGES 4

#define MY_CHILD_SENSOR_ID 0

MyMessage msg(MY_CHILD_SENSOR_ID, V_STATUS);

void presentation()
{
	sendSketchInfo("MySensors Follower Node", __DATE__);
	present(MY_CHILD_SENSOR_ID, S_BINARY, "Receiver");
}

void receive(const MyMessage& message)
{
	static uint8_t lastValue = 0;

	// We only expect one type of message from controller. But we better check anyway.
	if (message.type == V_STATUS) {
		if (message.sender == MY_PARENT_NODE_ID) {

			uint8_t dataSize = message.getLength();
			uint8_t* data = message.getCustom();

			// Send response to sender
			msg.set(data, dataSize);
			msg.setDestination(MY_PARENT_NODE_ID);
			send(msg);

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

void setup()
{
	// Activate debug pins
	pinMode(DEBUG_PIN1, OUTPUT);
	pinMode(DEBUG_PIN2, OUTPUT);
	pinMode(DEBUG_PIN3, OUTPUT);

	randomSeed(analogRead(A4)); // Analog pin A4 shall be not connected
}

void loop()
{
	// Pin debug: loop start
	digitalWrite(DEBUG_PIN1, HIGH);

	// Pin debug: loop end
	digitalWrite(DEBUG_PIN1, LOW);
}
