#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mqtt.h"
#include <unistd.h>
MQTTClient mqtt_client;

int mqtt_init() {
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    int rc;

    MQTTClient_create(&mqtt_client, MQTT_ADDRESS, MQTT_CLIENTID, MQTTCLIENT_PERSISTENCE_NONE, NULL);
    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;

    if ((rc = MQTTClient_connect(mqtt_client, &conn_opts)) != MQTTCLIENT_SUCCESS) {
        printf("Failed to connect MQTT, return code %d\n", rc);
        return rc;
    }

    printf("MQTT connected to broker at %s\n", MQTT_ADDRESS);
    return 0;
}

void mqtt_publish(const char *msg) {
MQTTClient_message pubmsg = MQTTClient_message_initializer;
    MQTTClient_deliveryToken token;

    pubmsg.payload = (void*)msg;
    pubmsg.payloadlen = strlen(msg);
    pubmsg.qos = MQTT_QOS;
    pubmsg.retained = 1 ;// Retaine;d = 0 to avoid old messages

    MQTTClient_publishMessage(mqtt_client, MQTT_TOPIC, &pubmsg, &token);
    MQTTClient_waitForCompletion(mqtt_client, token, MQTT_TIMEOUT);
    printf("MQTT: Published -> %s\n", msg);
}






// void mqtt_publish(const char *msg) {
//     MQTTClient client;
//     MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
//     int rc;

//     MQTTClient_create(&client, ADDRESS, CLIENTID, MQTTCLIENT_PERSISTENCE_NONE, NULL);
//     conn_opts.keepAliveInterval = 20;
//     conn_opts.cleansession = 1;

//     if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS) {
//         printf("Failed to connect, return code %d\n", rc);
//         return;
//     }

//     MQTTClient_message pubmsg = MQTTClient_message_initializer;
//     pubmsg.qos = QOS;
//     pubmsg.retained = 0;
//    // while(1){
//     pubmsg.payload = (void*)msg;
//     pubmsg.payloadlen = (int)strlen(msg);
    

//     MQTTClient_deliveryToken token;
//     MQTTClient_publishMessage(client, TOPIC, &pubmsg, &token);
//     MQTTClient_waitForCompletion(client, token, TIMEOUT);

//     printf("MQTT: Published to topic %s -> %s\n", TOPIC, msg);

// //sleep(4);
//     //}
//     MQTTClient_disconnect(client, 10000);
//     MQTTClient_destroy(&client);
// 
// }