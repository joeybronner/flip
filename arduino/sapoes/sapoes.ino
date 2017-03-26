/* Couleurs (format RGB) */
const byte COLOR_BLACK = 0b000;
const byte COLOR_RED = 0b100;
const byte COLOR_GREEN = 0b010;
const byte COLOR_BLUE = 0b001;
const byte COLOR_MAGENTA = 0b101;
const byte COLOR_CYAN = 0b011;
const byte COLOR_YELLOW = 0b110;
const byte COLOR_WHITE = 0b111;

/* Broches */
const byte PIN_LED_R = 9;
const byte PIN_LED_G = 10;
const byte PIN_LED_B = 11;
const byte PIN_BUT = 5;
int val = 0;
int incomingByte = 0;

void setup() {
    // Initialize the LED pins as outputs
    pinMode(PIN_LED_R, OUTPUT);
    pinMode(PIN_LED_G, OUTPUT);
    pinMode(PIN_LED_B, OUTPUT);
    pinMode(PIN_BUT, INPUT);
    displayColor(COLOR_WHITE);
    Serial.begin(9600);
}
 
void loop() {
    if (Serial.available() > 0) { incomingByte = Serial.read(); }
 
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

void displayColor(byte color) {

  // Version anode commune
  digitalWrite(PIN_LED_R, !bitRead(color, 2));
  digitalWrite(PIN_LED_G, !bitRead(color, 1));
  digitalWrite(PIN_LED_B, !bitRead(color, 0));
}
