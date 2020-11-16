#include <MQTT.h>
#include <MQTTClient.h>
#include <WiFi.h>
#include <ESP32Servo.h>

const char ssid[] = "SIZZLE-DESIGN";
const char pass[] = "sizzle256";
WiFiClient net;
MQTTClient client;
Servo motorServo;
int servo = 18;
int pos = 0;

unsigned long lastMillis = 0;

void connect() {
  Serial.print("checking wifi...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }

  Serial.print("\nconnecting...");
  while (!client.connect("esp32-miniproject03", "504b31ec", "d2e925056242a119")) {
    Serial.print(".");
    delay(1000);
  }

  Serial.println("\nconnected!");

  client.subscribe("/button");
  client.subscribe("/servomessage");
  // client.unsubscribe("/hello");
}

void messageReceived(String &topic, String &payload) {
  Serial.println("incoming: " + topic + " - " + payload);
  if(topic.equals("/button")){
    if(payload.equals("1")){
      for (pos = 0; pos <= 360; pos += 1) { // goes from 0 degrees to 180 degrees
        // in steps of 1 degree
        Serial.println("Servo is run anti-clockwise");
        motorServo.write(pos);    // tell servo to go to position in variable 'pos'
      }
      client.publish("/servomessage", "Servo is run anti-clockwise");
    }
    else if(payload.equals("0")){
      for (pos = 360; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
        Serial.println("Servo is run clockwise");
        motorServo.write(pos);    // tell servo to go to position in variable 'pos'
      }
      client.publish("/servomessage", "Servo is run clockwise");
    }
  }
}

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, pass);
  
  // Allow allocation of all timers
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);
  
  motorServo.setPeriodHertz(50);    // standard 50 hz servo
  motorServo.attach(servo, 500, 2400); // attaches the servo on pin 13 to the servo object
  
  client.begin("broker.shiftr.io", net);
  client.onMessage(messageReceived);

  connect();
}

void loop() {
  client.loop();
  delay(10);  // <- fixes some issues with WiFi stability
  if (!client.connected()) {
    connect();
  }
}
