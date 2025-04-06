const int LED_RED = 13;    
const int BUTTON_PIN = 2;  // Button pin

const int DEFAULT_SPEED = 500; // Default blink delay (milliseconds)
const int FAST_SPEED = 100;    // Fast blink delay when button is held

int _blinkSpeed = DEFAULT_SPEED; 

void setup() {
    pinMode(LED_RED, OUTPUT);
    pinMode(BUTTON_PIN, INPUT);
}

void loop() {
  // Check if the button is pressed
    if (digitalRead(BUTTON_PIN) == HIGH) {
        _blinkSpeed = DEFAULT_SPEED;  // Faster blinking when pressed
    } 
    else {
        _blinkSpeed = FAST_SPEED;  // Normal blinking when released
    }

  // LED Blinking Logic
    digitalWrite(LED_RED, HIGH);
    delay(_blinkSpeed);
    digitalWrite(LED_RED, LOW);
    delay(blinkSpeed);
}