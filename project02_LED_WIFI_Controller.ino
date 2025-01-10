#include <WiFi.h>

// WiFi credentials
const char* ssid = "Hanuman";      // Replace with your WiFi SSID
const char* password = "25L6R0K6"; // Replace with your WiFi Password

// GPIO pin for the LED
const int ledPin = 15; // Adjust based on your circuit

WiFiServer server(80); // Create a web server on port 80

void setup() {
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW); // Initially off

  Serial.begin(115200);
  delay(1000);

  // Connect to WiFi
  Serial.print("Connecting to WiFi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Start the web server
  server.begin();
}

void checkWiFiConnection() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Reconnecting to WiFi...");
    WiFi.disconnect();
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    Serial.println("\nReconnected to WiFi");
  }
}

void loop() {
  checkWiFiConnection(); //check and reconnect WiFi if it gets disconnected
  WiFiClient client = server.available(); // Check for incoming client

  if (client) {
    String request = client.readStringUntil('\r');
    client.flush();

    // Handle LED control
    if (request.indexOf("/LED_ON") != -1) {
      digitalWrite(ledPin, HIGH);
      Serial.println("LED ON");
    } else if (request.indexOf("/LED_OFF") != -1) {
      digitalWrite(ledPin, LOW);
      Serial.println("LED OFF");
    }

    // Send response headers with CORS support
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/plain");
    client.println("Access-Control-Allow-Origin: *"); // CORS header
    client.println("Connection: close");
    client.println();
    client.println("OK");
    client.stop();
  }
}
