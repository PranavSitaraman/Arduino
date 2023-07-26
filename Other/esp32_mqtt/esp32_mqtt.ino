#include <Arduino.h>
#include <ArduinoMqttClient.h>
#include <WiFi.h>

///////please enter your sensitive data in the Secret tab/arduino_secrets.h
char ssid[] = "ssid";        // your network SSID (name)
char pass[] = "password";    // your network password (use for WPA, or use as key for WEP)

WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);

const char broker[] = "192.168.114.165";
int        port     = 1883;

int count = 0;

void onMqttMessage(int messageSize) {
  // we received a message, print out the topic and contents
  Serial.print("Topic: ");
  Serial.println(mqttClient.messageTopic());
  Serial.print("', length ");
  Serial.print(messageSize);
  Serial.println(" bytes:");

  // use the Stream interface to print the contents
  while (mqttClient.available()) {
    Serial.print((char)mqttClient.read());
  }
  Serial.println();
  Serial.println();
}

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.print("Connecting to WiFi");
  WiFi.begin(ssid, pass, 6);
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }
  Serial.println(" Connected!");
  Serial.println("You're connected to the network");
  Serial.println();

  Serial.print("Attempting to connect to the MQTT broker: ");
  Serial.println(broker);

  if (!mqttClient.connect(broker, port)) {
    Serial.print("MQTT connection failed! Error code = ");
    Serial.println(mqttClient.connectError());

    while (1);
  }

  Serial.println("You're connected to the MQTT broker!");
  Serial.println();
  mqttClient.onMessage(onMqttMessage);
  for (int i = 0; i < 100; i++)
  {
    volatile unsigned long currentMillis = millis();
    mqttClient.subscribe("topic");
    volatile unsigned long subscribe = millis() - currentMillis;
    Serial.print("Subscribe time: ");
    Serial.println(subscribe/1000.0);
  }
  mqttClient.beginMessage("topic1");
  mqttClient.print("test1");
  mqttClient.endMessage();

  mqttClient.beginMessage("topic2");
  mqttClient.print("test2");
  mqttClient.endMessage();

  mqttClient.beginMessage("topic3");
  mqttClient.print("test3");
  mqttClient.endMessage();
}

void loop() {
  mqttClient.poll();
}