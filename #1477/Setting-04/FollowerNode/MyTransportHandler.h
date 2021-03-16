// MySensors reliable transport HAL send/receive function handlers

#pragma once

bool transportHALSendHandler(const uint8_t nextRecipient, MyMessage* outMsg, const uint8_t len,
	const bool noACK)
{
	bool result = false;
	for (int i = 0; i < 8; i++) {

#if defined(DEBUG_PIN2)
		digitalWrite(DEBUG_PIN2, HIGH);
#endif
		result = transportHALSend(nextRecipient, outMsg, len, noACK);
#if defined(DEBUG_PIN2)
		digitalWrite(DEBUG_PIN2, LOW);
#endif
		if (result || noACK) {
			// Always delay after send to give RF24 radio
			// time to receive incoming messages
			delay(7);
			break;
		}
		uint8_t delta = random(0, 7) * 7;
		delay(delta);
	}
	return result;
}
