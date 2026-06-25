```c
#include <stdio.h>
#include <string.h>
#include "MQTTClient.h"

/* MQTT Configuration */
#define ATTR_TOPIC "v1/devices/me/attributes"

#define QOS 1
#define TIMEOUT 10000L

/* Functions from other modules */
void relay_on();
void relay_off();

void save_energy();

/* Global variables */
extern double energy;

/*--------------------------------------------------*/
/* Publish Attribute                                */
/*--------------------------------------------------*/

void publish_attribute(MQTTClient client,
                       const char *payload)
{
    MQTTClient_message msg =
        MQTTClient_message_initializer;

    MQTTClient_deliveryToken token;

    msg.payload = (void *)payload;
    msg.payloadlen = strlen(payload);
    msg.qos = QOS;
    msg.retained = 0;

    MQTTClient_publishMessage(
        client,
        ATTR_TOPIC,
        &msg,
        &token);

    MQTTClient_waitForCompletion(
        client,
        token,
        TIMEOUT);
}

/*--------------------------------------------------*/
/* ThingsBoard Callback                             */
/*--------------------------------------------------*/

int messageArrived(void *context,
                   char *topicName,
                   int topicLen,
                   MQTTClient_message *message)
{
    MQTTClient client = (MQTTClient)context;

    char payload[512];

    memset(payload, 0, sizeof(payload));

    memcpy(payload,
           message->payload,
           message->payloadlen);

    printf("\nRX [%s]\n%s\n",
           topicName,
           payload);

    /* Relay ON */
    if (strstr(payload, "\"relay\":true"))
    {
        relay_on();
    }

    /* Relay OFF */
    if (strstr(payload, "\"relay\":false"))
    {
        relay_off();
    }

    /* Energy Reset */
    if (strstr(payload, "\"reset_energy\":true"))
    {
        energy = 0.0;

        save_energy();

        printf("Energy Reset\n");

        /* Auto-clear reset button */
        publish_attribute(
            client,
            "{\"reset_energy\":false}");
    }

    MQTTClient_freeMessage(&message);
    MQTTClient_free(topicName);

    return 1;
}
```
