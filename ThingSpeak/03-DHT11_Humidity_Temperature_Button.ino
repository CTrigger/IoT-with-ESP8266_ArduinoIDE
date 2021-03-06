/*
Description:
Button Flash event to sent data
humidity and temperature
to ThingSpeak Service
*/
// Libraries
#include <ESP8266WiFi.h>
#include <DHT.h>

// WiFi settings
const char *ssid =  "YOUR SSID";
const char *password =  "YOUR PASS";

//API
const char* host = "api.thingspeak.com";
String ApiKey = "xxxxxxxxxxxxxxxx";
String path = "/update?key=" + ApiKey; 

//Interface
int BTN_FLASH = 0;
int LED = 2;
int DHT11Pin = 5;
DHT dht(DHT11Pin, DHT11);

//Data
String DATA = "";

void setup() {
  //Serial setup
  Serial.begin(9600);

  //Interface setup
  pinMode(BTN_FLASH, INPUT);
  pinMode(LED, OUTPUT);
  
  //WiFi Conecting
  Serial.print("Connecting... ");
  WiFi.begin(ssid, password);
  while(WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(100);
  }
  Serial.println();
  Serial.println("Connected: " + String(ssid));
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  digitalWrite(LED, digitalRead(BTN_FLASH));
  
  // Triggered in button Event
  if (digitalRead(BTN_FLASH) == LOW){  
    float humidity = dht.readHumidity();
    float temperature = dht.readTemperature();
    
    //test Reading
    if(isnan(humidity) || isnan(temperature)){
      Serial.println("Error: Unable receive data from Sensor");
      return;
      
    }else{
      String field1 = "&field1=" + String(humidity);
      String field2 = "&field2=" + String(temperature);
      
      DATA = "";
      DATA += "POST " + String(path) + " HTTP/1.1\r\n";
      DATA += "Host: " + String(host) + "\r\n";
      DATA += "Connection: close\r\n";
      DATA += "X-THINGSPEAKAPIKEY:" + ApiKey + "\r\n";
      DATA += "Content-Type: application/x-www-form-urlencoded\r\n";
      DATA += "Content-Length: " + String((field1+field2).length())+ "\r\n";
      DATA += "\r\n";
      DATA += field1 + field2;
      
      WiFiClient client;
      Serial.println("Requesting the API Service");
      if(!client.connect(host, 80)){
        Serial.println("Connection failed...");
        return;
      }else{
        Serial.println("Connection Granted");
      }
    
      Serial.println("Sending Data: \r\n" + DATA);
      client.println(DATA);
      Serial.println("Data Sent");

      Serial.println("Humidity: " + String(humidity));
      Serial.println("Temperature: " + String(temperature));
      
    }
  }
}
