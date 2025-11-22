// mqtt.h
#ifndef MQTT_H
#define MQTT_H
#include "MQTTClient.h"
void mqtt_publish(const char *msg);
void mqtt_subscribe(const char *topic);
int mqtt_init();
void log_message(const char* prefix, const char* msg, int priority);

extern MQTTClient mqtt_client;

#define BUFFER_SIZE 1024
#define MQTT_ADDRESS "tcp://test.mosquitto.org:1883"
#define MQTT_CLIENTID "PersistentServerPub"
#define MQTT_TOPIC   "proxelera/data"
#define MQTT_QOS     1
#define MQTT_TIMEOUT 10000L

#endif
