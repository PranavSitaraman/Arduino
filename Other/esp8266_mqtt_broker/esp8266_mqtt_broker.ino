#include <ESP8266WiFi.h>
#include "uMQTTBroker.h"
uMQTTBroker myBroker;
char ssid[] = "ssid";        // network SSID
char pass[] = "password";    // network password
class myMQTTBroker: public uMQTTBroker
{
public:
    virtual bool onConnect(IPAddress addr, uint16_t client_count) {
      Serial.println(addr.toString()+" connected");
      return true;
    }
    virtual void onDisconnect(IPAddress addr, String client_id) {
      Serial.println(addr.toString()+" ("+client_id+") disconnected");
    }
    virtual bool onAuth(String username, String password, String client_id) {
      Serial.println("Username/Password/ClientId: "+username+"/"+password+"/"+client_id);
      return true;
    }
    virtual void onData(String topic, const char *data, uint32_t length) {
      char data_str[length+1];
      os_memcpy(data_str, data, length);
      data_str[length] = '\0';
      Serial.println("Received topic '"+topic+"' with data '"+(String)data_str+"'");
    }
    virtual void printClients() {
      for (int i = 0; i < getClientCount(); i++) {
        IPAddress addr;
        String client_id;
        getClientAddr(i, addr);
        getClientId(i, client_id);
        Serial.println("Client "+client_id+" on addr: "+addr.toString());
      }
    }
};
void startWiFiClient()
{
  Serial.println("Connecting to WiFi");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: " + WiFi.localIP().toString());
}
void setup()
{
  uart_div_modify(0, UART_CLK_FREQ / 115200);
  Serial.begin(115200);
  Serial.println();
  startWiFiClient();
  Serial.println("Starting MQTT broker");
  myBroker.init();
}
int counter = 0;
void loop()
{
  delay(1000);
}