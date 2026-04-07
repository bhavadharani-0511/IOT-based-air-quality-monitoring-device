#include <ESP8266WiFi.h>

const char* ssid = "bhava";
const char* password = "bhava123";

WiFiServer server(80);

int sensorPin = A0;
int buzzerPin = D5;

int threshold = 1000;

void setup() {

  Serial.begin(115200);
  delay(1000);

  pinMode(buzzerPin, OUTPUT);
  digitalWrite(buzzerPin, LOW);

  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi...");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi Connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  server.begin();
}

void loop() {

  int airValue = analogRead(sensorPin);

  Serial.print("Air Quality Value: ");
  Serial.println(airValue);

  // 🔊 LOUD ACTIVE BUZZER LOGIC
  if (airValue > threshold) {
    
    // Fast ON/OFF pattern for strong alert sound
    digitalWrite(buzzerPin, HIGH);
    delay(200);
    digitalWrite(buzzerPin, LOW);
    delay(100);
    
  } else {
    digitalWrite(buzzerPin, LOW);
  }

  WiFiClient client = server.available();
  if (!client) {
    delay(500);
    return;
  }

  // Simple Web Page
  String html = "<!DOCTYPE html><html><body>";
  html += "<h2>Air Quality Monitor</h2>";
  html += "<p>Air Value: ";
  html += airValue;
  html += "</p></body></html>";

  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("Connection: close");
  client.println();
  client.println(html);

  delay(1);
  client.stop();
}