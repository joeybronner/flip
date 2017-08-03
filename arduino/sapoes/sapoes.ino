#include <MQTT.h>
#include <PubSubClient.h>
#include <PubSubClient_JSON.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>


// Colors (RGB)
const byte COLOR_BLACK = 0b000;
const byte COLOR_RED = 0b100;
const byte COLOR_GREEN = 0b010;
const byte COLOR_BLUE = 0b001;
const byte COLOR_MAGENTA = 0b101;
const byte COLOR_CYAN = 0b011;
const byte COLOR_YELLOW = 0b110;
const byte COLOR_WHITE = 0b111;

// MQTT
const char *mqtt_server = "m10.cloudmqtt.com";
const int mqtt_port = 17316;
const char *mqtt_user = "flip-joey";
const char *mqtt_pass = "helloworld";

// Broches RGB LED
const int PIN_LED_R = D1;
const int PIN_LED_G = D2;
const int PIN_LED_B = D3;

// Button
//const byte PIN_BUT = 5;

// WiFi credentials
char ssid[] = "freebox_ALMEIDA";
char pass[] = "ziugtiojciech6onuecu";

int val = 0;
int incomingByte = 0;

WiFiClient wclient;
PubSubClient client(wclient, mqtt_server, mqtt_port);
#define BUFFER_SIZE 100

void setup() {

    // Debug console
    Serial.begin(9600);
    delay(200);
    Serial.println("");
    Serial.println("NodeMCU Started");

    // Initialize the LED pins as outputs
    pinMode(PIN_LED_R, OUTPUT);
    pinMode(PIN_LED_G, OUTPUT);
    pinMode(PIN_LED_B, OUTPUT);

    //pinMode(PIN_BUT, INPUT);
}
 
void loop() {
    if (WiFi.status() != WL_CONNECTED) {
        connectWifi();
        if (WiFi.waitForConnectResult() != WL_CONNECTED) {
            blinkColor(3000, COLOR_RED);
            return;
        } else {
            displayColor(COLOR_GREEN);
            delay(3000);
            displayColor(COLOR_BLACK);
            Serial.println("WiFi connected");
        }
        
    }
    if (Serial.available() > 0) {
        incomingByte = Serial.read();
        if (incomingByte == 'g') {
            displayColor(COLOR_GREEN);
        } else if (incomingByte == 'r') {
            displayColor(COLOR_RED);
        } else if (incomingByte == 'y') {
            displayColor(COLOR_YELLOW);
        } else {
            Serial.write("Incoming byte : " + incomingByte);
        }
    }
    if (!client.connected()) {
        Serial.println("Connecting to MQTT server");
        if (client.connect(MQTT::Connect("arduinoClient2").set_auth(mqtt_user, mqtt_pass))) {
            Serial.println("Connected to MQTT server");
            client.set_callback(callback);
            client.subscribe("flip/devices/1/status");
        } else {
            Serial.println("Could not connect to MQTT server");   
        }
    }

    if (client.connected()) {
        client.loop();
    }
}

void callback(const MQTT::Publish& pub) {
    Serial.print(pub.topic());
    Serial.print(" => ");
    if (pub.has_stream()) {
        uint8_t buf[BUFFER_SIZE];
        int read;
        while (read = pub.payload_stream()->read(buf, BUFFER_SIZE)) {
          Serial.write(buf, read);
        }
        pub.payload_stream()->stop();
        Serial.println("");
    } else {
        Serial.println(pub.payload_string());  
    }
}

void displayColor(byte color) {
  // Version anode commune
  digitalWrite(PIN_LED_R, !bitRead(color, 2));
  digitalWrite(PIN_LED_G, !bitRead(color, 1));
  digitalWrite(PIN_LED_B, !bitRead(color, 0));
}

void connectWifi() {
    Serial.print("Connecting to ");
    Serial.print(ssid);
    Serial.println("...");
    WiFi.begin(ssid, pass);
}

void blinkColor(int t, byte color) {
  int times = t / 500;
  for (int i=0; i <= times; i++){
      displayColor(color);
      delay(250);
      displayColor(COLOR_BLACK);
      delay(250);
   }
}
