```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include "MQTTClient.h"

/* MQTT Configuration */
#define ADDRESS   "tcp://192.168.0.10:1883"
#define CLIENTID  "RuggedBoard"
#define TOKEN     "1Fhkamm6bVY4kHQZaoeY"

#define TELEMETRY_TOPIC "v1/devices/me/telemetry"
#define ATTR_TOPIC      "v1/devices/me/attributes"

#define QOS 1
#define TIMEOUT 10000L

/* Functions from other modules */
int read_adc();

void relay_on();
void relay_off();

void save_energy();
void load_energy();

void publish_attribute(MQTTClient client,
                       const char *payload);

int messageArrived(void *context,
                   char *topicName,
                   int topicLen,
                   MQTTClient_message *message);

double calculate_current(int raw);
double calculate_power(double current);
double update_energy(double energy,
                     double power,
                     double elapsed);

/* Global variables defined elsewhere */
extern double energy;
extern int relay_status;

int main()
{
    MQTTClient client;

    MQTTClient_connectOptions conn_opts =
        MQTTClient_connectOptions_initializer;

    load_energy();

    MQTTClient_create(
        &client,
        ADDRESS,
        CLIENTID,
        MQTTCLIENT_PERSISTENCE_NONE,
        NULL);

    MQTTClient_setCallbacks(
        client,
        client,
        NULL,
        messageArrived,
        NULL);

    conn_opts.username = TOKEN;
    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;

    if (MQTTClient_connect(
            client,
            &conn_opts)
        != MQTTCLIENT_SUCCESS)
    {
        printf("Connection failed\n");
        return -1;
    }

    printf("Connected to ThingsBoard\n");

    MQTTClient_subscribe(
        client,
        ATTR_TOPIC,
        QOS);

    /* Request latest shared attributes */
    const char *attr_req =
        "{\"sharedKeys\":\"relay,reset_energy\"}";

    MQTTClient_message reqmsg =
        MQTTClient_message_initializer;

    MQTTClient_deliveryToken reqtoken;

    reqmsg.payload = (void *)attr_req;
    reqmsg.payloadlen = strlen(attr_req);
    reqmsg.qos = QOS;

    MQTTClient_publishMessage(
        client,
        "v1/devices/me/attributes/request/1",
        &reqmsg,
        &reqtoken);

    MQTTClient_waitForCompletion(
        client,
        reqtoken,
        TIMEOUT);

    time_t last_time = time(NULL);

    while (1)
    {
        int raw = read_adc();

        if (raw < 0)
        {
            printf("ADC read failed\n");
            sleep(2);
            continue;
        }

        /* Calculate electrical parameters */
        double current =
            calculate_current(raw);

        double power =
            calculate_power(current);

        time_t now = time(NULL);

        double elapsed =
            difftime(now, last_time);

        last_time = now;

        energy =
            update_energy(
                energy,
                power,
                elapsed);

        save_energy();

        char payload[256];

        snprintf(
            payload,
            sizeof(payload),
            "{\"current\":%.3f,"
            "\"power\":%.2f,"
            "\"energy\":%.6f,"
            "\"relay_status\":%d}",
            current,
            power,
            energy,
            relay_status);

        MQTTClient_message pubmsg =
            MQTTClient_message_initializer;

        MQTTClient_deliveryToken token;

        pubmsg.payload = payload;
        pubmsg.payloadlen = strlen(payload);
        pubmsg.qos = QOS;
        pubmsg.retained = 0;

        MQTTClient_publishMessage(
            client,
            TELEMETRY_TOPIC,
            &pubmsg,
            &token);

        MQTTClient_waitForCompletion(
            client,
            token,
            TIMEOUT);

        printf(
            "Current=%.3f A  "
            "Power=%.2f W  "
            "Energy=%.6f kWh  "
            "Relay=%d\n",
            current,
            power,
            energy,
            relay_status);

        sleep(2);
    }

    MQTTClient_disconnect(client, 1000);
    MQTTClient_destroy(&client);

    return 0;
}
```
