#include <Wire.h>
#include <Adafruit_MLX90614.h>
#include <WiFi.h>
#include <HTTPClient.h>

Adafruit_MLX90614 mlx = Adafruit_MLX90614();

// WiFi credentials
const char* ssid = "M14";
const char* password = "12345677";

// ThingSpeak API details
String apiKey = "9T59OK6TJXCF94MP";
const char* server = "http://api.thingspeak.com/update";

void setup() {
  Serial.begin(115200);

  // Start sensor
  if (!mlx.begin()) {
    Serial.println("Error connecting to MLX90614 sensor. Check wiring!");
    while (1);
  }

  // Connect WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected!");
}

void loop() {
  float ambientTemp = mlx.readAmbientTempC();
  float objectTemp = mlx.readObjectTempC();

  Serial.print("Ambient = "); Serial.print(ambientTemp);
  Serial.print(" °C | Object = "); Serial.println(objectTemp);

  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String url = String(server) + "?api_key=" + apiKey +
                 "&field1=" + String(ambientTemp) +
                 "&field2=" + String(objectTemp);

    http.begin(url.c_str());
    int httpCode = http.GET();

    if (httpCode > 0) {
      Serial.println("Data sent successfully!, Mr. Dangwal");
    } else {
      Serial.println("Error sending data!");
    }
    http.end();
  }
  delay(15000); // ThingSpeak accepts data every 15 sec minimum
}
