#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid ="1";         // replace with your Wi-Fi SSID
const char* password ="11111111"; // replace with your Wi-Fi password

const char* botToken = "8597828149:AAF7LXJvjFAoBRQc9jO-QiKkm7rHDqmS_M4";     // replace with your Telegram bot token
const char* chatID = "7863531839";           // your chat ID

const int trigPin = 5;  // Ultrasonic trig pin
const int echoPin = 18; // Ultrasonic echo pin

long duration;
int distance;

void setup() {
  Serial.begin(115200);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi");
}

void loop() {
  // Measure distance
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;

  Serial.print("Distance: ");
  Serial.println(distance);

  if (distance < 50) { // Threshold in cm
    sendTelegramMessage("⚠️ Object detected! Distance: " + String(distance) + " cm");
    delay(2000); // Wait 5 seconds before sending next message
  }
  delay(500);
}

void sendTelegramMessage(String message) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String url = "https://api.telegram.org/bot" + String(botToken) + "/sendMessage?chat_id=" + String(chatID) + "&text=" + message;
    http.begin(url);
    int httpResponseCode = http.GET();
    if (httpResponseCode > 0) {
      Serial.println("Message sent successfully");
    } else {
      Serial.println("Error sending message");
    }
    http.end();
  }
}
