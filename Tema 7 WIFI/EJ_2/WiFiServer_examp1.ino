#include <WiFi.h>

const char* ssid = "Your_SSID"; // Replace with your WiFi SSID
const char* password = "Your_PASSWORD"; // Replace with your WiFi Password

WiFiServer server(80); // Create a server on port 80

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password); // Connect to WiFi

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  
  Serial.println("Connected to WiFi");
  Serial.print("Server IP Address: ");
  Serial.println(WiFi.localIP());

  server.begin(); // Start the server
}

void loop() {
  WiFiClient client = server.available(); // Check for incoming clients

  if (client) {
    Serial.println("New Client Connected");
    while (client.connected()) {
      if (client.available()) {
        String request = client.readStringUntil('\r');
        Serial.println(request);
        client.flush();

        // Respond with a basic HTML message
        client.print("HTTP/1.1 200 OK\nContent-Type: text/html\n");
        client.print("Connection: close\n\n");
        client.print("<html><body><h1>Hello from ESP32 Server!</h1></body></html>");
        break;
      }
    }
    client.stop();
    Serial.println("Client Disconnected");
  }
}
