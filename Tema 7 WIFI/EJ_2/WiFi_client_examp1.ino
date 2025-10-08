#include <WiFi.h>

const char* ssid = "Your_SSID"; // Replace with your WiFi SSID
const char* password = "Your_PASSWORD"; // Replace with your WiFi Password
const char* serverIP = "SERVER_IP_ADDRESS"; // Replace with the server's IP address

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password); // Connect to WiFi

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  
  Serial.println("Connected to WiFi");
}

void loop() {
  WiFiClient client;

  Serial.print("Connecting to server...");
  if (client.connect(serverIP, 80)) { // Connect to the server
    Serial.println("Connected!");
    
    // Send an HTTP GET request
    client.print("GET / HTTP/1.1\r\n");
    client.print("Host: ");
    client.print(serverIP);
    client.print("\r\n");
    client.print("Connection: close\r\n\r\n");
    
    // Wait for the response
    while (client.connected() || client.available()) {
      if (client.available()) {
        String line = client.readStringUntil('\n');
        Serial.println(line);
      }
    }
    
    client.stop();
    Serial.println("Disconnected from server");
  } else {
    Serial.println("Connection failed");
  }

  delay(10000); // Wait 10 seconds before the next request
}
