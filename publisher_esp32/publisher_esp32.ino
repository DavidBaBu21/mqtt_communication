 //This code publishes the information reicieved by the esp32 to red node
//Commited
#include <Wire.h>
#include <PubSubClient.h>
#include <WiFi.h>

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

float temperatura = 0;

const char* ssid = "Totalplay-2.4G-44a8";
const char* password = "";
const char* mqtt_server = "192.168.100.16";

void setup(){
  Serial.begin(115200);
  setup_wifi();
  delay(500);
  client.setServer("192.168.100.16", 1883);
//  client.setCallback(callback);
}

void setup_wifi(){
  delay(10);
  Serial.println();
  Serial.print("Conectando a ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.println(".");
  }
  Serial.println("");
  Serial.println("WiFi conectado");
  Serial.println("IP adress: ");
  Serial.println(WiFi.localIP());
}

void reconnect(){
  while(!client.connected()){
    Serial.print("Trying MQTT conection...");
    if(client.connect("ESP32Cliet")){
      Serial.println("Connected");
    }else{
      Serial.print("Fallo, rc=");
      Serial.print(client.state());
      Serial.println("Intentelo de nuevo en 5s");
      delay(5000);
    }
    client.loop();
  }
}

void loop(){
  if(!client.connected()){
    reconnect(); 
  }
  client.loop();
  
  long now = millis();
  if (now - lastMsg > 1000){
    lastMsg = now;

    temperatura = analogRead(36)*(100.0/4095.0);
    char tempString[8];
    dtostrf(temperatura, 1, 2, tempString);
    Serial.print("Temperatura: ");
    Serial.println(tempString);
    client.publish("esp32/temperature", tempString);
  }
}
