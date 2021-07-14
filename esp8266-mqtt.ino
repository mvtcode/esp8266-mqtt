#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// example code: http://iot47.com/iot-bai-7-esp8266-arduino-ide-va-giao-thuc-mqtt/

// Thông tin về wifi
// #define ssid "huy"
// #define password "huy07092001"
#define ssid "Icetea's Home"
#define password "Icetea@2018"
#define mqtt_server "broker.hivemq.com"
const uint16_t mqtt_port = 1883; //Port của CloudMQTT TCP

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(9600);
  Serial.println("Goodnight moon!");
  pinMode(LED_BUILTIN, OUTPUT);
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port); 
  client.setCallback(callback);
}

// Hàm kết nối wifi
void setup_wifi() {
  delay(10);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  
  Serial.println(WiFi.localIP());
}

// Hàm call back để nhận dữ liệu
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Co tin nhan moi tu topic:");
  Serial.println(topic);
  for (int i = 0; i < length; i++)
    Serial.print((char) payload[i]);
  Serial.println();
  
  digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
  Serial.println("Led change status:" + digitalRead(LED_BUILTIN));
}

// Hàm reconnect thực hiện kết nối lại khi mất kết nối với MQTT Broker
void reconnect() {
  while (!client.connected()) { // Chờ tới khi kết nối
    // Thực hiện kết nối với mqtt user và pass # https://pubsubclient.knolleary.net/api#connect
    // clientID = ESP8266_ID1 | willTopic = ESP_offline | willQoS = 0 | willRetain = 0 | willMessage = ESP8266_id1_offline
    if (client.connect("ESP8266_ID1", "ESP_offline", 0, 0, "ESP8266_id1_offline")) {  //kết nối vào broker
      // https://pubsubclient.knolleary.net/api#subscribe
      client.subscribe("ESP8266_ID1_TEST"); //đăng kí nhận dữ liệu từ topic ESP8266_ID1_TEST
    } else {
      delay(5000);
    }
  }
}

unsigned long t;
void loop() {
  if (!client.connected()) { // Kiểm tra kết nối
    reconnect();
  }
  
  client.loop();
  if(millis() - t > 2000) { //nếu 2000 mili giây trôi qua
     t = millis();
     client.publish("ESP8266_ID1_TEST", "Bay gio la 12h"); // gửi dữ liệu lên topic ESP8266_ID1_TEST
  }
}
