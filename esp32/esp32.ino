#include <WiFi.h>
#include <WebSocketsServer.h>

const char* ssid = "vikram";
const char* password = "000000000";

WebSocketsServer webSocket = WebSocketsServer(81);
const int ledPin = 2;  // On-board LED (GPIO 2 for most ESP32)

void onWebSocketEvent(uint8_t client_num, WStype_t type, uint8_t * payload, size_t length) {
  switch(type) {
    case WStype_DISCONNECTED:
      Serial.printf("Client #%u disconnected\n", client_num);
      break;
      
    case WStype_CONNECTED:
      Serial.printf("Client #%u connected from IP: %s\n", client_num, webSocket.remoteIP(client_num).toString().c_str());
      webSocket.sendTXT(client_num, "Connected to ESP32!");
      break;
      
    case WStype_TEXT:
      String msg = String((char*)payload);
      Serial.println("Received from client #" + String(client_num) + ": " + msg);
      
      if (msg == "ON") {
        digitalWrite(ledPin, HIGH);
        webSocket.sendTXT(client_num, "✅ LED is ON");
        Serial.println("LED turned ON");
      } 
      else if (msg == "OFF") {
        digitalWrite(ledPin, LOW);
        webSocket.sendTXT(client_num, "❌ LED is OFF");
        Serial.println("LED turned OFF");
      }
      else if (msg == "STATUS") {
        String status = digitalRead(ledPin) ? "ON" : "OFF";
        webSocket.sendTXT(client_num, "LED Status: " + status);
      }
      else {
        webSocket.sendTXT(client_num, "❌ Unknown command: " + msg);
      }
      break;
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW); // Initially OFF
  
  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println();
  Serial.println("✅ WiFi connected!");
  Serial.println("IP address: " + WiFi.localIP().toString());
  Serial.println("WebSocket server starting on port 81...");
  
  webSocket.begin();
  webSocket.onEvent(onWebSocketEvent);
  
  Serial.println("🚀 ESP32 WebSocket LED Controller ready!");
  Serial.println("Open the HTML file and update the IP address to: " + WiFi.localIP().toString());
}

void loop() {
  webSocket.loop();
  
  // Optional: Add a heartbeat every 30 seconds
  static unsigned long lastHeartbeat = 0;
  if (millis() - lastHeartbeat > 30000) {
    Serial.println("💓 System running... Connected clients: " + String(webSocket.connectedClients()));
    lastHeartbeat = millis();
  }
}