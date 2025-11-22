#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>          
#include "MQTTClient.h"

#define ADDRESS     "tcp://test.mosquitto.org:1883"
#define CLIENTID    "ExampleClientSub"
#define TOPIC       "proxelera/data"
#define QOS         1
#define TIMEOUT     10000L    

int main() {
    MQTTClient client;
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    int rc;

    // Create MQTT client
    MQTTClient_create(&client, ADDRESS, CLIENTID, MQTTCLIENT_PERSISTENCE_NONE, NULL);
    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;

    // Connect to broker
    if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS) {
        printf("Failed to connect, return code %d\n", rc);
        return rc;
    }

    printf("Connected to broker at %s\n", ADDRESS);

    // Subscribe to topic
    MQTTClient_subscribe(client, TOPIC, QOS);
    printf("Subscribed to topic: %s\n", TOPIC);
    printf("Waiting for messages...\n");

    // Loop to receive messages
    while (1) {
        char* topicName = NULL;
        int topicLen;
        MQTTClient_message* message = NULL;

        // Receive message (blocking, with timeout)
        rc = MQTTClient_receive(client, &topicName, &topicLen, &message, 1000); // 1000ms timeout

        if (rc == MQTTCLIENT_SUCCESS && message != NULL) {
            printf("\nMessage received:\n");
            printf("   Topic: %s\n", topicName);
            printf("   Message: %.*s\n", message->payloadlen, (char*)message->payload);

            // Free resources
            MQTTClient_freeMessage(&message);
            MQTTClient_free(topicName);
        }

        // Optional: avoid busy loop
        usleep(100000); // 0.1 second
    }

    // Disconnect and cleanup (never reached in this example)
    MQTTClient_disconnect(client, 10000);
    MQTTClient_destroy(&client);

    return 0;
}
