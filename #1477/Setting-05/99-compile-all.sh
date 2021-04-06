#!/bin/bash
arduino-cli compile LeaderNode -u
arduino-cli compile FollowerNode -u
arduino-cli compile Repeater -u
arduino-cli compile GatewayESP8266 -u
