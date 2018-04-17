#include <PubSubClient.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <BlynkSimpleEsp8266.h>
#include <Adafruit_NeoPixel.h>

// TODO

/*
 * - no data during 1 minute, switch off the light
 */


// MQTT
const char *mqtt_server = "m23.cloudmqtt.com";
const int mqtt_port = 12358;
const char *mqtt_user = "joey";
const char *mqtt_pass = "password";

// RGB LED
#define PIN D2
#define NUMPIXELS 1
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(1, PIN, NEO_GRB + NEO_KHZ800);

// Button
const int PIN_PUSH_BT = D4; // TODO: ADAPT
long buttonTimer = 0;
long longPressTime = 2000;
boolean buttonActive = false;
boolean longPressActive = false;

// WiFi credentials
static WiFiClientSecure network;
const char* KNOWN_SSID[]     = {"iPhone de Joey Bronner", "SAP-Guest", "SFR-97f0"};
const char* KNOWN_PASSWORD[] = {"joeyjoey", "", "Q54CH9NNKU4A"};
const int   KNOWN_SSID_COUNT = sizeof(KNOWN_SSID) / sizeof(KNOWN_SSID[0]);

// Timer
BlynkTimer timer;

// Mode
bool autoMode = true;
String manualStatus = "r";

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
    Serial.println("Wemos Started");

    // Switch on LED blue while connnecting to the WiFi
    pixels.begin();
    delay(100);
    switchColor("b");
    delay(100);

    // WiFi before launch
    initWiFiConnection();

    //pinMode(PIN_PUSH_BT, INPUT);

    // Setup a function to be called every second
    timer.setInterval(30000L, checkConnection);
}

void loop() {

    timer.run();

    /*if (digitalRead(PIN_PUSH_BT) == HIGH) {
        Serial.println("Button pressed");
        switchColorManual();
        delay(200);
    }*/

    if (!client.connected() && WiFi.status() == WL_CONNECTED) {
        Serial.println("Connecting to MQTT server");
        if (client.connect(MQTT::Connect("arduinoClient2").set_auth(mqtt_user, mqtt_pass))) {
            Serial.println("Connected to MQTT server");
            client.set_callback(callback);
            String top = "flip/devices/1/status";
            client.subscribe(top);
            Serial.println("Subscribed on " + top);
        } else {
            Serial.println("Could not connect to MQTT server");
        }
    }

    if (client.connected() && autoMode) {
        client.loop();
    }
}

void checkConnection () {
    int timeout = millis();
    if (WiFi.status() == WL_CONNECTED) {
        Serial.println('Connected');
    } else {
        digitalWrite(BUILTIN_LED, 0);
        initWiFiConnection();
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
        Serial.println("Color: " + color);
        switchColor(color);
    } else {
        Serial.println(pub.payload_string());
        switchColor(pub.payload_string());
    }
}

/*
 * LED Utils
 */

void switchColorManual() {
    if (manualStatus == "r") {
        switchColor(manualStatus);
        manualStatus = "g";
    } else if (manualStatus == "g") {
        switchColor(manualStatus);
        manualStatus = "r";
    }/* else if (manualStatus == "b") {
        switchColor(manualStatus);
        manualStatus = "m";
    } else if (manualStatus == "m") {
        switchColor(manualStatus);
        manualStatus = "c";
    } else if (manualStatus == "c") {
        switchColor(manualStatus);
        manualStatus = "y";
    } else if (manualStatus == "y") {
        switchColor(manualStatus);
        manualStatus = "w";
    } else if (manualStatus == "w") {
        switchColor(manualStatus);
        manualStatus = "r";
    }*/
}

void switchColor(String c) {
    if (c == "r")
          displayColor(255, 0, 0);
    if (c == "g")
          displayColor(0, 255, 0);
    if (c == "b")
          displayColor(0, 0, 255);
    if (c == "m")
          displayColor(80, 0, 80);
    if (c == "c")
          displayColor(0, 255, 255);
    if (c == "y")
          displayColor(255, 255, 0);
    if (c == "w")
          displayColor(255, 255, 255);
    if (c == "o")
          displayColor(0, 0, 0);
}

void displayColor(int red, int green, int blue) {
  pixels.setPixelColor(0, pixels.Color(red,green,blue));
  pixels.show();
  delay(2000);
}

void blinkColor(int t, String c) {
  int times = t / 500;
  for (int i=0; i <= times; i++){
      switchColor(c);
      delay(250);
      switchColor("o");
      delay(250);
   }
}

void initWiFiConnection () {
  digitalWrite(BUILTIN_LED, 0);
  boolean wifiFound = false;
  int i, n;

  // ----------------------------------------------------------------
  // Set WiFi to station mode and disconnect from an AP if it was previously connected
  // ----------------------------------------------------------------
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  Serial.println("Setup done");

  // ----------------------------------------------------------------
  // WiFi.scanNetworks will return the number of networks found
  // ----------------------------------------------------------------
  Serial.println(F("scan start"));
  int nbVisibleNetworks = WiFi.scanNetworks();
  Serial.println(F("scan done"));
  if (nbVisibleNetworks == 0) {
    Serial.println(F("no networks found. Reset to try again"));
    while (true); // no need to go further, hang in there, will auto launch the Soft WDT reset
  }

  // ----------------------------------------------------------------
  // if you arrive here at least some networks are visible
  // ----------------------------------------------------------------
  Serial.print(nbVisibleNetworks);
  Serial.println(" network(s) found");

  // ----------------------------------------------------------------
  // check if we recognize one by comparing the visible networks
  // one by one with our list of known networks
  // ----------------------------------------------------------------
  for (i = 0; i < nbVisibleNetworks; ++i) {
    Serial.println(WiFi.SSID(i)); // Print current SSID
    for (n = 0; n < KNOWN_SSID_COUNT; n++) { // walk through the list of known SSID and check for a match
      if (strcmp(KNOWN_SSID[n], WiFi.SSID(i).c_str())) {
        Serial.print(F("\tNot matching "));
        Serial.println(KNOWN_SSID[n]);
      } else { // we got a match
        wifiFound = true;
        break; // n is the network index we found
      }
    } // end for each known wifi SSID
    if (wifiFound) break; // break from the "for each visible network" loop
  } // end for each visible network

  if (!wifiFound) {
    Serial.println(F("no Known network identified. Reset to try again"));
    while (true); // no need to go further, hang in there, will auto launch the Soft WDT reset
  }

  // ----------------------------------------------------------------
  // if you arrive here you found 1 known SSID
  // ----------------------------------------------------------------
  Serial.print(F("\nConnecting to "));
  Serial.println(KNOWN_SSID[n]);

  // ----------------------------------------------------------------
  // We try to connect to the WiFi network we found
  // ----------------------------------------------------------------
  WiFi.begin(KNOWN_SSID[n], KNOWN_PASSWORD[n]);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");

  // ----------------------------------------------------------------
  // SUCCESS, you are connected to the known WiFi network
  // ----------------------------------------------------------------
  digitalWrite(BUILTIN_LED, 1);
  delay(150);
  Serial.println(F("WiFi connected, your IP address is "));
  Serial.println(WiFi.localIP());
  switchColor("o");
}
