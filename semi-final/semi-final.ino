#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <DHT.h>

#define DHTPIN 2  // D3 
#define DHTTYPE DHT11
#define CURRENT_SENSOR_POWER 5  // D1
#define VOLTAGE_SENSOR_POWER 4  // D2
#define SENSOR_PIN A0  // Entrée analogique
#define LED1 14  // LED 1
#define LED2 12  // LED 2
#define LED3 13  // LED 3

const char* ssid = "inwi Home 4G 8C52A5";
const char* password = "5N32832JG2";

ESP8266WebServer server(80);
DHT dht(DHTPIN, DHTTYPE);

void setup() {
    Serial.begin(115200);
    dht.begin();
    
    pinMode(CURRENT_SENSOR_POWER, OUTPUT);
    pinMode(VOLTAGE_SENSOR_POWER, OUTPUT);
    pinMode(LED1, OUTPUT);
    pinMode(LED2, OUTPUT);
    pinMode(LED3, OUTPUT);
    
    digitalWrite(CURRENT_SENSOR_POWER, LOW);
    digitalWrite(VOLTAGE_SENSOR_POWER, LOW);
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, LOW);
    digitalWrite(LED3, LOW);

    WiFi.begin(ssid, password);
    Serial.print("Connecting to WiFi...");
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.print(".");
    }
    Serial.println("\nConnected!");
    Serial.println(WiFi.localIP());

    server.on("/", HTTP_OPTIONS, []() {
      server.sendHeader("Access-Control-Allow-Origin", "*");
      server.sendHeader("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
      server.sendHeader("Access-Control-Allow-Headers", "Content-Type");
      server.send(200, "text/plain", "");
    });

    server.on("/data", HTTP_OPTIONS, []() {
        server.sendHeader("Access-Control-Allow-Origin", "*");
        server.sendHeader("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
        server.sendHeader("Access-Control-Allow-Headers", "Content-Type");
        server.send(200, "text/plain", "");
    });
    server.on("/data", HTTP_GET, []() {
        float temperature = dht.readTemperature();
        float humidity = dht.readHumidity();
        float current = readCurrent();
        float voltage = readVoltage();

        String jsonData = "{";
        jsonData += "\"temperature\":" + String(temperature, 2) + ",";
        jsonData += "\"humidity\":" + String(humidity, 2) + ",";
        jsonData += "\"current\":" + String(current, 2) + ",";
        jsonData += "\"voltage\":" + String(voltage, 2);
        jsonData += "}";

        server.sendHeader("Access-Control-Allow-Origin", "*");  
        server.sendHeader("Access-Control-Allow-Methods", "GET, POST, OPTIONS");  
        server.sendHeader("Access-Control-Allow-Headers", "Content-Type"); 
        server.send(200, "application/json", jsonData);
    });

    server.on("/led1/on", HTTP_GET, []() {
        digitalWrite(LED1, HIGH);
        server.sendHeader("Access-Control-Allow-Origin", "*");  
        server.sendHeader("Access-Control-Allow-Methods", "GET, POST, OPTIONS");  
        server.sendHeader("Access-Control-Allow-Headers", "Content-Type");
        server.send(200, "text/plain", "LED 1 ON");
    });

    server.on("/led1/off", HTTP_GET, []() {
        digitalWrite(LED1, LOW);
        server.sendHeader("Access-Control-Allow-Origin", "*");  
        server.sendHeader("Access-Control-Allow-Methods", "GET, POST, OPTIONS");  
        server.sendHeader("Access-Control-Allow-Headers", "Content-Type");
        server.send(200, "text/plain", "LED 1 OFF");
    });

    server.on("/led2/on", HTTP_GET, []() {
        digitalWrite(LED2, HIGH);
        server.sendHeader("Access-Control-Allow-Origin", "*");  
        server.sendHeader("Access-Control-Allow-Methods", "GET, POST, OPTIONS");  
        server.sendHeader("Access-Control-Allow-Headers", "Content-Type");
        server.send(200, "text/plain", "LED 2 ON");
    });

    server.on("/led2/off", HTTP_GET, []() {
        digitalWrite(LED2, LOW);
        server.sendHeader("Access-Control-Allow-Origin", "*");  
        server.sendHeader("Access-Control-Allow-Methods", "GET, POST, OPTIONS");  
        server.sendHeader("Access-Control-Allow-Headers", "Content-Type");
        server.send(200, "text/plain", "LED 2 OFF");
    });

    server.on("/led3/on", HTTP_GET, []() {
        digitalWrite(LED3, HIGH);
        server.sendHeader("Access-Control-Allow-Origin", "*");  
        server.sendHeader("Access-Control-Allow-Methods", "GET, POST, OPTIONS");  
        server.sendHeader("Access-Control-Allow-Headers", "Content-Type");
        server.send(200, "text/plain", "LED 3 ON");
    });

    server.on("/led3/off", HTTP_GET, []() {
        digitalWrite(LED3, LOW);
        server.sendHeader("Access-Control-Allow-Origin", "*");  
        server.sendHeader("Access-Control-Allow-Methods", "GET, POST, OPTIONS");  
        server.sendHeader("Access-Control-Allow-Headers", "Content-Type");
        server.send(200, "text/plain", "LED 3 OFF");
    });

    server.begin();
    Serial.println("Server started.");
}

void loop() {
    server.handleClient();
    yield();  // Évite que l'ESP se bloque
}

float readCurrent() {
    digitalWrite(VOLTAGE_SENSOR_POWER, LOW);
    delay(50);
    digitalWrite(CURRENT_SENSOR_POWER, HIGH);
    delay(50);
    int sensorValue = analogRead(SENSOR_PIN);
    digitalWrite(CURRENT_SENSOR_POWER, LOW);
    float voltage = sensorValue * (3.3 / 1023.0);
    float current = (voltage - 2.5) / 0.185; 
    return current;
}

float readVoltage() {
    digitalWrite(CURRENT_SENSOR_POWER, LOW);
    delay(50);
    digitalWrite(VOLTAGE_SENSOR_POWER, HIGH);
    delay(50);
    int sensorValue = analogRead(SENSOR_PIN);
    digitalWrite(VOLTAGE_SENSOR_POWER, LOW);
    float voltage = sensorValue * (3.3 / 1023.0);  
    float measuredVoltage = voltage * (5.0);  
    return measuredVoltage;
}
