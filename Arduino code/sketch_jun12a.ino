#include <WiFi.h>
#include <HTTPClient.h>


// Replace with your network credentials
const char* ssid = "FIK-Dekanat";
const char* password = "F4silkom";

// Server URL
const char* serverName = "http://127.0.0.1:5000//sensor-data";

// Sensor pin
const int mq135Pin = 34; // Adjust based on your wiring

// Relay pin
const int relayPin = 32;

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  pinMode(mq135Pin, INPUT);
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, LOW);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi");
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    int sensorValue = analogRead(mq135Pin);
    Serial.println(sensorValue);
    
    // Reading sensor data
    int airQuality = analogRead(mq135Pin);

    http.begin(serverName);
    http.addHeader("Content-Type", "application/json");

    // JSON data
    String jsonData = "{\"air_quality\":" + String(airQuality) + "}";

    // Sending HTTP POST request
    int httpResponseCode = http.POST(jsonData);

    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println(httpResponseCode);
      Serial.println(response);

      // Parse response to control the fan
      if (response.indexOf("turn_on_fan") != -1) {
        digitalWrite(relayPin, HIGH);
      } else if (response.indexOf("turn_off_fan") != -1) {
        digitalWrite(relayPin, LOW);
      }
    } else {
      Serial.print("Error on sending POST: ");
      Serial.println(httpResponseCode);
    }

    http.end();
  } else {
    Serial.println("WiFi Disconnected");
  }

  // Delay before the next reading
  delay(2000); // Send data every 30 seconds
}
