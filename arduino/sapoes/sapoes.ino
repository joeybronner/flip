#include <MQTT.h>
#include <PubSubClient.h>
#include <PubSubClient_JSON.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>

// TODO

/*
 * - no data during 1 minute, switch off the light
 */

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
const String deviceId = "1";

// Broches RGB LED
const int PIN_LED_R = D1;
const int PIN_LED_G = D2;
const int PIN_LED_B = D3;

// Button
const int PIN_PUSH_BT = D4;
long buttonTimer = 0;
long longPressTime = 250;
boolean buttonActive = false;
boolean longPressActive = false;

// WiFi credentials
char ssid[] = "SAP-Guest";
char pass[] = "";

// Mode
bool autoMode = true;
String manuelStatus = "r";

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

    pinMode(PIN_PUSH_BT, INPUT);
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
    /*if (Serial.available() > 0) {
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
    }*/
    /*if (digitalRead(PIN_PUSH_BT) == HIGH) {
        if (buttonActive == false) {
            buttonActive = true;
            buttonTimer = millis();
        }
        if ((millis() - buttonTimer > longPressTime) && (longPressActive == false)) {
            longPressActive = true;
            autoMode = !autoMode;
            waitingMode();
        } else {
            switchColorManual();
        }
    }*/
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

    if (client.connected() && autoMode) {
        client.loop();
    }
}

/*
 * MQTT Callback
 */

void callback(const MQTT::Publish& pub) {
    //Serial.print(pub.topic());
    //Serial.print(" => ");
    if (pub.has_stream()) {
        uint8_t buf[BUFFER_SIZE];
        int read;
        while (read = pub.payload_stream()->read(buf, BUFFER_SIZE)) {
            //Serial.write(buf, read);
        }
        pub.payload_stream()->stop();
        String color = (char*)buf;
        switchColor(color);
    } else {
        Serial.println(pub.payload_string());
        switchColor(pub.payload_string());
    }
}

/*
 * WiFi Utils
 */

void connectWifi() {
    Serial.print("Connecting to ");
    Serial.print(ssid);
    Serial.println("...");
    WiFi.begin(ssid, pass);
}

/*
 * LED Utils
 */

void switchColorManual() {
    if (manuelStatus == "r") {
        displayColor(COLOR_RED);
        manuelStatus = "g";
    } else if (manuelStatus == "g") {
        displayColor(COLOR_GREEN);
        manuelStatus = "b";
    } else if (manuelStatus == "b") {
        displayColor(COLOR_BLUE);
        manuelStatus = "m";
    } else if (manuelStatus == "m") {
        displayColor(COLOR_MAGENTA);
        manuelStatus = "c";
    } else if (manuelStatus == "c") {
        displayColor(COLOR_CYAN);
        manuelStatus = "y";
    } else if (manuelStatus == "y") {
        displayColor(COLOR_YELLOW);
        manuelStatus = "w";
    } else if (manuelStatus == "w") {
        displayColor(COLOR_WHITE);
        manuelStatus = "r";
    }
}

void switchColor(String c) {
    if (c == "r") 
          displayColor(COLOR_RED);
    if (c == "g")
          displayColor(COLOR_GREEN);
    if (c == "b")
          displayColor(COLOR_BLUE);
    if (c == "m")
          displayColor(COLOR_MAGENTA);
    if (c == "c")
          displayColor(COLOR_CYAN);
    if (c == "y")
          displayColor(COLOR_YELLOW);
    if (c == "w")
          displayColor(COLOR_WHITE);
}

void displayColor(byte color) {
  // Version anode commune
  digitalWrite(PIN_LED_R, !bitRead(color, 2));
  digitalWrite(PIN_LED_G, !bitRead(color, 1));
  digitalWrite(PIN_LED_B, !bitRead(color, 0));
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

void waitingMode() {
    displayColor(COLOR_RED);
    delay(200);
    displayColor(COLOR_GREEN);
    delay(200);
    displayColor(COLOR_BLUE);
    delay(200);
    displayColor(COLOR_MAGENTA);
    delay(200);
    displayColor(COLOR_CYAN);
    delay(200);
    displayColor(COLOR_YELLOW);
    delay(200);
    displayColor(COLOR_WHITE);
    delay(200);
}
