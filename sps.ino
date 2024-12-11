#include <WiFi.h>

const char* ssid = "IQOO Neo7 Pro";
const char* password = "chiragb18";

WiFiServer server(80);

const int IN1 = 13;
const int IN2 = 12;
const int t=26;
const int IN3 = 14;
const int IN4 = 27;


void setup() {
  Serial.begin(115200);
  Serial.println("Starting setup...");

  // Configure motor control pins
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(t,OUTPUT);
  


  // Ensure motors are stopped
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);

  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.print("Connected to WiFi. IP address: ");
  Serial.println(WiFi.localIP());

  server.begin();
  Serial.println("Server started");
}

void stopMotors() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void moveForward() {
  stopMotors();
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void moveBackward() {
  stopMotors();
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void turnLeft() {
  stopMotors();
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void turnRight() {
  stopMotors();
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}
void taser(){
  stopMotors();
  digitalWrite(t,HIGH);
  delay(1000);
  digitalWrite(t,LOW);
  delay(1000);
}

void loop() {
  WiFiClient client = server.available();

  if (client) {
    Serial.println("New Client Connected");
    String currentLine = "";
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        if (c == '\n') {
          currentLine.trim();
          Serial.print("Command received: ");
          Serial.println(currentLine);

          if (currentLine == "0") {
            Serial.println("Command 0: Stopping");
            stopMotors();
          } else if (currentLine == "1") {
            Serial.println("Command 1: Forward");
            moveForward();
          } else if (currentLine == "2") {
            Serial.println("Command 2: Backward");
            moveBackward();
          } else if (currentLine == "3") {
            Serial.println("Command 3: Left");
            turnLeft();
          } else if (currentLine == "4") {
            Serial.println("Command 4: Right");
            turnRight();
          }
          else if (currentLine == "5") {
            Serial.println("Command 5:BOOM");
            taser();

          currentLine = "";
        } else if (c != '\r') {
          currentLine += c;
        }
      }
    }
    client.stop();
    Serial.println("Client Disconnected.");
  }
}
}