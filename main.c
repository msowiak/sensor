//
// BME280 sample app
//
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>
#include "bme280.h"
#include "MQTTClient.h"
// delay between samples in microseconds
#define DELAY 1000000
// Width of the graph (128 - 40)
#define WIDTH 88
int main(int argc, char *argv[])
{
int i;
int T, P, H; // calibrated values

// Test code for mqtt
if (argc != 2) {
	printf("Usage: bmeapp mqtt_address \n");
	return -1;
}

MQTTClient client;
int rc = 0;

rc = MQTTClient_create(&client, argv[1], "csensorapp", MQTTCLIENT_PERSISTENCE_NONE, NULL);

MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
conn_opts.keepAliveInterval = 10;
conn_opts.cleansession = 1;
//conn_opts.username = "username";
//conn_opts.password = "password";

rc = MQTTClient_connect(client, &conn_opts);
//printf(rc);

char* payload;
int payloadlen = strlen(payload);
int qos = 1;
int retained = 0;
MQTTClient_deliveryToken dt;
rc = MQTTClient_publish(client, "sensor_test", payloadlen, payload, qos, retained, &dt);

	i = bme280Init(0, 0x76);
	if (i != 0)
	{
		return -1; // problem - quit
	}
	printf("BME280 device successfully opened.\n");
	usleep(1000000); // wait for data to settle for first read

	for (i=0; i<120; i++) // read values twice a second for 1 minute
	{
		bme280ReadValues(&T, &P, &H);
		T -= 150; // for some reason, the sensor reports temperatures too high
		printf("Calibrated temp. = %3.2f C, pres. = %6.2f Pa, hum. = %2.2f%%\n", (float)T/100.0, (float)P/256.0, (float)H/1024.0);
		sprintf(payload, "Calibrated temp. = %3.2f C, pres. = %6.2f Pa, hum. = %2.2f%%\n", (float)T/100.0, (float)P/256.0, (float)H/1024.0);
		rc = MQTTClient_publish(client, "sensor_test", payloadlen, payload, qos, retained, &dt);
		usleep(DELAY);
	}

return 0;
} /* main() */
