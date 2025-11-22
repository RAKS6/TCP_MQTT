# TCP to MQTT Bridge

This project implements a **TCP server** that receives messages from multiple TCP clients and forwards them to an **MQTT broker**. It also includes example TCP clients and a Python test script to verify the functionality.

## Features

- **TCP Server:** Listens on `127.0.0.1:8080` and accepts multiple clients concurrently.
- **MQTT Integration:** Publishes received TCP messages to the MQTT topic `proxelera/data` on `test.mosquitto.org`.
- **Example Clients:** `client.c` and `client2.c` send messages to the server.
- **Python Test Script:** Uses `pytest` and `paho-mqtt` to simulate clients and verify that messages are received on MQTT.

## How to Run

1. Compile and start the TCP server:
   ```bash
   gcc server.c mqtt.c -o server -lpaho-mqtt3c -lpthread
   ./server
