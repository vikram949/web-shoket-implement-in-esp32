#include <WiFi.h>
#include <WebSocketsServer.h>

const char* ssid = "vikram";
const char* password = "000000000";

WebSocketsServer webSocket = WebSocketsServer(81);
const int ledPin = 2;  // On-board LED (GPIO 2 for most ESP32)

void onWebSocketEvent(uint8_t client_num, WStype_t type, uint8_t * payload, size_t length) {
  if (type == WStype_TEXT) {
    String msg = String((char*)payload);
    Serial.println("Received: " + msg);

    if (msg == "ON") {
      digitalWrite(ledPin, HIGH);
    } else if (msg == "OFF") {
      digitalWrite(ledPin, LOW);
    }

    webSocket.sendTXT(client_num, "LED is now " + msg);
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi: " + WiFi.localIP().toString());

  webSocket.begin();
  webSocket.onEvent(onWebSocketEvent);
}

void loop() {
  webSocket.loop();
}
