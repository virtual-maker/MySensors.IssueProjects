#include "arduino_secrets.h"

// Enable debug prints to serial monitor
#define MY_DEBUG
// Enable support for I_DEBUG messages.
#define MY_SPECIAL_DEBUG

// Enables and select radio type (if attached)
#define MY_RADIO_RF24
// Set transmission channel
#define MY_RF24_CHANNEL SECRET_RF24_CHANNEL

#define MY_GATEWAY_ESP8266

#define MY_WIFI_SSID SECRET_WIFI_SSID
#define MY_WIFI_PASSWORD SECRET_WIFI_PASSWORD

// Set the host name for the WiFi Client
#define MY_HOSTNAME SECRET_HOSTNAME

// Enable MY_IP_ADDRESS here if you want a static IP address (no DHCP)
//#define MY_IP_ADDRESS SECRET_IP_ADDRESS

// If using static IP you can define Gateway and Subnet address as well
//#define MY_IP_GATEWAY_ADDRESS SECRET_IP_GATEWAY_ADDRESS
//#define MY_IP_SUBNET_ADDRESS SECRET_IP_SUBNET_ADDRESS

// The port to keep open on node server mode
#define MY_PORT 5003

// How many clients should be able to connect to this gateway (default 1)
#define MY_GATEWAY_MAX_CLIENTS 2

#include <MySensors.h>

void presentation()
{
    sendSketchInfo("MySensors Gateway", "1.0");
}

void receive(const MyMessage& message)
{
}

void setup()
{
}

void loop()
{
}
