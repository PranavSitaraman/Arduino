#include <Arduino.h>
#include <ArduinoMqttClient.h>
#include <WiFi.h>
char ssid[] = "ssid";        // network SSID
char pass[] = "password";    // network password
WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);
const char broker[] = "mqtt.eclipseprojects.io";
int port = 1883;
int count = 0;
void onMqttMessage(int messageSize)
{
  Serial.print("Topic: '");
  Serial.print(mqttClient.messageTopic());
  Serial.print("', length ");
  Serial.print(messageSize);
  Serial.print(", bytes: '");
  while (mqttClient.available())
  {
    Serial.print((char)mqttClient.read());
  }
  Serial.println("'");
}
void setup()
{
  Serial.begin(9600);
  while (!Serial);
  WiFi.begin(ssid, pass, 6);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(100);
  }
  Serial.println();
  Serial.println("WiFi Connected");
  Serial.println("Connecting to MQTT Broker");
  if (!mqttClient.connect(broker, port))
  {
    Serial.print("MQTT connection failed!");
    while (1);
  }
  Serial.println("MQTT Connected");
  mqttClient.onMessage(onMqttMessage);
  for (int i = 0; i < 100; i++)
  {
    volatile unsigned long currentMillis = millis();
    mqttClient.subscribe("topic");
    volatile unsigned long subscribe = millis() - currentMillis;
    Serial.print("Subscribe time (ms): ");
    Serial.println(subscribe);
  }
  mqttClient.subscribe("topic1");
  for (int i = 0; i < 100; i++)
  {
    volatile unsigned long currentMillis = millis();    
    mqttClient.beginMessage("topic1");
    mqttClient.print("test1");
    mqttClient.endMessage();
    volatile unsigned long publish = millis() - currentMillis;
    Serial.print("Publish time (ms): ");
    Serial.println(publish);
  }
}
void loop()
{
  mqttClient.poll();
}