#define BLYNK_TEMPLATE_ID "TMPL31u36QwLQ"
#define BLYNK_TEMPLATE_NAME "project"
#define BLYNK_AUTH_TOKEN "ZYek6Q2JyaD2tCwaW5h01VqEDHz_3Zyb"
char ssid[] = "Test";
char pass[] = "Test12345";
#include <BlynkSimpleEsp8266.h>
#include <ESP8266WiFi.h>



#define IN1 5  // Motor A input 1
#define IN2 4  // Motor A input 2
#define IN3 0  // Motor B input 1
#define IN4 2  // Motor B input 2

#define TRIG_PIN 14  // Ultrasonic sensor trigger pin
#define ECHO_PIN 12  // Ultrasonic sensor echo pin
#define MAX_DISTANCE 40  // Distance threshold in cm

int xValue = 0;
int yValue = 0;
bool blynkMode = true;

long measureDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  
  long duration = pulseIn(ECHO_PIN, HIGH);
  long distance = duration * 0.034 / 2;
  
  return distance;
}

void controlMotors(int x, int y) {
  if (y > 80) {  // Forward
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
  }
  else if (y < 20) {  // Backward
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
  }
  else if (x > 80) {  // Right
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
  }
  else if (x < 20) {  // Left
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
  }
  else {  // Stop
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
  }
}

void obstacleAvoidance() {
  int distance = measureDistance();
  if (distance > 0 && distance < MAX_DISTANCE) {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
  } else {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
  }
}

BLYNK_WRITE(V1) {
  if (blynkMode) {
    xValue = param[0].asInt();
    yValue = param[1].asInt();
    controlMotors(xValue, yValue);
  }
}

BLYNK_WRITE(V2) {
  blynkMode = param.asInt();
  if (!blynkMode) {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
  }
}

void setup() {
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  Serial.begin(9600);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
}

void loop() {
  Blynk.run();
  
  if (!blynkMode) {
    obstacleAvoidance();
  }
}